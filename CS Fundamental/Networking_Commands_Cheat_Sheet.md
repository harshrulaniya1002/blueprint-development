# Networking Commands Cheat Sheet

## Interfaces & IP

```bash
ifconfig
ip addr
ip a
ipconfig getifaddr en0
route get default
ip route
netstat -rn
```

Use cases:
- `ifconfig` — view or configure network interfaces on older systems.
- `ip addr` / `ip a` — show IP addresses and interface details on Linux.
- `ipconfig getifaddr en0` — get the IP address of a specific interface on macOS.
- `route get default` — check the default gateway.
- `ip route` / `netstat -rn` — inspect the routing table.

## ARP / MAC

```bash
arp -a
ip neigh
ip neigh flush all
```

Use cases:
- `arp -a` — display MAC-to-IP mappings on the local network.
- `ip neigh` — inspect the ARP/neighbor cache.
- `ip neigh flush all` — clear stale neighbor entries.

## DNS

```bash
nslookup google.com
dig google.com
dig -x 8.8.8.8
host google.com
```

Use cases:
- `nslookup google.com` — query DNS records manually.
- `dig google.com` — get detailed DNS information.
- `dig -x 8.8.8.8` — perform reverse DNS lookup.
- `host google.com` — quickly resolve a hostname to an IP.

## Connectivity

```bash
ping google.com
traceroute google.com
tracert google.com
mtr google.com
```

Use cases:
- `ping google.com` — test reachability and round-trip time.
- `traceroute google.com` — find the path and hops to a destination.
- `tracert google.com` — Windows equivalent of traceroute.
- `mtr google.com` — monitor latency and packet loss continuously.

## Ports & Connections

```bash
netstat -an
netstat -tulpn
ss -tulpn
lsof -i
lsof -i :8080
lsof -iTCP -sTCP:LISTEN
```

Use cases:
- `netstat -an` — list active TCP/UDP connections and listening ports.
- `netstat -tulpn` — show listening ports and the programs using them.
- `ss -tulpn` — modern replacement for `netstat`.
- `lsof -i` — find processes using network connections.
- `lsof -i :8080` — check who is using a specific port.
- `lsof -iTCP -sTCP:LISTEN` — find processes listening for TCP connections.

## Packet Capture

```bash
sudo tcpdump -i en0
sudo tcpdump port 80
sudo tcpdump port 443
sudo tcpdump port 22
sudo tcpdump arp
sudo tcpdump host 8.8.8.8
```

Use cases:
- `sudo tcpdump -i en0` — capture packets on a specific interface.
- `sudo tcpdump port 80` — inspect HTTP traffic.
- `sudo tcpdump port 443` — inspect HTTPS traffic.
- `sudo tcpdump port 22` — inspect SSH traffic.
- `sudo tcpdump arp` — filter ARP packets.
- `sudo tcpdump host 8.8.8.8` — capture traffic to or from a specific host.

## HTTP / APIs

```bash
curl https://google.com
curl -v https://google.com
curl -I https://google.com
wget https://example.com/file.zip
```

Use cases:
- `curl https://google.com` — send a basic HTTP request.
- `curl -v https://google.com` — debug request and response details.
- `curl -I https://google.com` — fetch only headers.
- `wget https://example.com/file.zip` — download files from a URL.

## SSH

```bash
ssh user@host
ssh -v user@host
scp file user@host:/path
```

Use cases:
- `ssh user@host` — connect to a remote machine securely.
- `ssh -v user@host` — debug SSH connection problems.
- `scp file user@host:/path` — copy files to a remote server.

## Network Testing

```bash
nc host port
telnet host port
```

Use cases:
- `nc host port` — test TCP connectivity to a port.
- `telnet host port` — check if a service is listening and interact with it.

## Network Configuration

```bash
hostname
hostname -I
networksetup -listallhardwareports
```

Use cases:
- `hostname` — print the device hostname.
- `hostname -I` — show the host IP addresses.
- `networksetup -listallhardwareports` — list network interfaces on macOS.

## Performance

```bash
iperf3 -s
iperf3 -c <server>
```

Use cases:
- `iperf3 -s` — start an iperf3 server for bandwidth testing.
- `iperf3 -c <server>` — test network throughput to a server.

## Firewall

```bash
sudo ufw status
sudo ufw allow 8080
sudo pfctl -sr
```

Use cases:
- `sudo ufw status` — check firewall status.
- `sudo ufw allow 8080` — open a port in the firewall.
- `sudo pfctl -sr` — view PF firewall rules.

## Wi-Fi

```bash
airport -I
iwconfig
nmcli device wifi list
```

Use cases:
- `airport -I` — show current Wi-Fi status on macOS.
- `iwconfig` — inspect wireless interface configuration on Linux.
- `nmcli device wifi list` — list available Wi-Fi networks.

## Public IP

```bash
curl ifconfig.me
curl https://api.ipify.org
whois google.com
```

Use cases:
- `curl ifconfig.me` / `curl https://api.ipify.org` — check your public IP address.
- `whois google.com` — look up domain registration details.

## Monitoring

```bash
watch netstat -an
watch ss -tulpn
```

Use cases:
- `watch netstat -an` — continuously monitor active connections.
- `watch ss -tulpn` — continuously monitor listening sockets.

# Top 15 Must-Know Commands

```bash
ifconfig
ip addr
route get default
ip route
arp -a
ping
traceroute
nslookup
dig
netstat -an
ss -tulpn
lsof -i
tcpdump
curl -v
ssh -v
```

Use cases:
- `ifconfig` — inspect interface configuration.
- `ip addr` — view IP addresses and interface state.
- `route get default` — find the default gateway.
- `ip route` — view routing information.
- `arp -a` — check local network MAC mappings.
- `ping` — verify connectivity to a host.
- `traceroute` — trace the network path.
- `nslookup` — query DNS records manually.
- `dig` — perform advanced DNS lookups.
- `netstat -an` — inspect active connections.
- `ss -tulpn` — see listening ports and services.
- `lsof -i` — identify processes using the network.
- `tcpdump` — capture and analyze packets.
- `curl -v` — debug HTTP requests.
- `ssh -v` — troubleshoot SSH connections.
