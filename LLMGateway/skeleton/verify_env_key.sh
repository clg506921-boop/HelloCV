#!/usr/bin/env bash
# 密钥读取与验证脚本（只输出指纹，绝不输出密钥本身）
#
# 用法：
#     cd skeleton
#     set -a; . ../.env; set +a      # 载入密钥（.env 已被 gitignore）
#     bash verify_env_key.sh
#
# 本脚本演示三件事：
#   1. 密钥从环境变量读到（gateway/config.py 的 resolved_api_key()）；
#   2. 读到的密钥确实被放进 Authorization 头；
#   3. 反向验证：故意用错密钥会得到 HTTP 401（证明鉴权链路真的通了）。
set -u

HERE="$(cd "$(dirname "$0")" && pwd)"
PY="${PYTHON:-$HERE/../.venv/bin/python}"
[ -x "$PY" ] || PY="$(command -v python3)"

cat > /tmp/_vek_key.py <<'PYEOF'
import hashlib, sys
sys.path.insert(0, sys.argv[1])
from gateway.config import GatewayConfig

cfg = GatewayConfig()
key = cfg.resolved_api_key()
print("  环境变量名:", cfg.api_key_env)
print("  是否读到密钥:", bool(key))
if key:
    print("  长度: %d  前 6 位: %s  后 4 位: %s" % (len(key), key[:6] + "***", "***" + key[-4:]))
    print("  指纹 sha256[:8]:", hashlib.sha256(key.encode()).hexdigest()[:8])
else:
    print("  长度: 0（未读到，请先 set -a; . ../.env; set +a）")
print("  上游地址:", cfg.api_base_url)
print("  默认模型:", cfg.model)
PYEOF

cat > /tmp/_vek_header.py <<'PYEOF'
import sys
sys.path.insert(0, sys.argv[1])
from gateway.client import LlmClient
from gateway.config import GatewayConfig

client = LlmClient(GatewayConfig(), log=lambda _m: None)
auth = client.build_headers().get("Authorization", "")
print("  有 Authorization 头:", bool(auth))
print("  形态为 'Bearer ***':", auth.startswith("Bearer ") and len(auth) > 10)
print("  与配置读到的一致:", auth == "Bearer " + client.api_key)
print("  端点:", client.endpoint)
client.close()
PYEOF

cat > /tmp/_vek_badkey.py <<'PYEOF'
import sys
sys.path.insert(0, sys.argv[1])
from gateway.client import LlmAuthError, LlmClient
from gateway.config import GatewayConfig

client = LlmClient(
    GatewayConfig(api_key="sk-" + "0" * 32, max_retries=0),
    sleep=lambda _s: None,
    log=lambda _m: None,
)
try:
    client.decide({"time": "t"})
    print("  意外：错误密钥竟然成功了")
except LlmAuthError:
    print("  收到 LlmAuthError（HTTP 401）——鉴权链路真的打到了官方 API")
except Exception as exc:  # noqa: BLE001
    print("  其它异常:", type(exc).__name__)
finally:
    client.close()
PYEOF

echo "=== 1. 密钥读取（只显示指纹） ==="
"$PY" /tmp/_vek_key.py "$HERE"

echo
echo "=== 2. 密钥是否进了 Authorization 头 ==="
"$PY" /tmp/_vek_header.py "$HERE"

echo
echo "=== 3. 反向验证：错误密钥应得到 401 ==="
"$PY" /tmp/_vek_badkey.py "$HERE"

echo
echo "=== 4. 版本控制检查 ==="
if git -C "$HERE/.." rev-parse --git-dir >/dev/null 2>&1; then
    echo -n "  .env 被忽略（不会提交）: "
    git -C "$HERE/.." check-ignore -q .env && echo "是" || echo "否（危险！）"
    echo -n "  .env.example 可入库: "
    git -C "$HERE/.." check-ignore -q .env.example && echo "否（被忽略了）" || echo "是"
fi
echo "VERIFY DONE"
