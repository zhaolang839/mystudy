可以按下面顺序检查 Ubuntu“没有网络”的原因：

1. 检查网卡是否存在并启用

```
ip -br link
ip -br addr
nmcli device status
```

正常情况下，网卡应显示 `UP` 或 `connected`，并拥有类似 `192.168.x.x` 的 IP。

Wi‑Fi 还需检查是否被禁用：

```
rfkill list
nmcli radio wifi
```

1. 检查默认网关

```
ip route
```

应看到类似：

```
default via 192.168.1.1 dev eth0
```

没有 `default` 路由通常表示 DHCP、网卡配置或网关有问题。

1. 分层测试连接

```
ping -c 4 127.0.0.1
ping -c 4 192.168.1.1      # 替换为 ip route 显示的网关
ping -c 4 1.1.1.1
ping -c 4 ubuntu.com
```

判断方法：

- 网关不通：网卡、Wi‑Fi、网线、虚拟机网络或路由器问题。
- 网关通，但 `1.1.1.1` 不通：上级网络、路由或防火墙问题。
- `1.1.1.1` 通，但域名不通：DNS 问题。
- 全部能通，但浏览器不能上网：检查代理、防火墙或浏览器配置。

1. 检查 DNS

```
resolvectl status
getent hosts ubuntu.com
cat /etc/resolv.conf
```

如果 DNS 异常，可临时测试：

```
sudo resolvectl dns <网卡名> 1.1.1.1 8.8.8.8
```

网卡名可从 `ip -br link` 获取，例如 `ens33`、`enp0s3` 或 `wlan0`。

1. 查看网络服务和日志

```
systemctl status NetworkManager --no-pager
journalctl -u NetworkManager -b -n 100 --no-pager
```

服务器版也可能使用：

```
systemctl status systemd-networkd --no-pager
sudo netplan get
```

1. 检查代理和防火墙

```
env | grep -i proxy
sudo ufw status verbose
```

如果需要重新连接 NetworkManager：

```
nmcli connection show
sudo nmcli connection up "<连接名称>"
```

如果你是通过 SSH 远程检查，先不要执行 `restart NetworkManager` 或 `netplan apply`，否则可能断开远程连接。