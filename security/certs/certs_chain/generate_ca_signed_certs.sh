#!/bin/bash
# ==============================================================
# Generate a full certificate chain: Root CA → Server/Client certs
# All FIPS 140-3 compliant (RSA 2048+, SHA-256)
# ==============================================================

# Prevent Git Bash (MSYS2) from mangling /subj paths on Windows
export MSYS_NO_PATHCONV=1

set -e
mkdir -p ca server client

# ========================
# 1. ROOT CA
# ========================
echo "=== Generating Root CA ==="

# Generate Root CA private key (RSA 4096 — FIPS approved)
openssl genrsa -out ca/ca.key 4096

# Generate Root CA self-signed certificate (10 years)
openssl req -new -x509 \
  -key ca/ca.key \
  -sha256 \
  -days 3650 \
  -out ca/ca.crt \
  -subj "/C=US/ST=California/L=SanFrancisco/O=MyOrg/OU=Security/CN=MyOrg Root CA"

echo "Root CA created: ca/ca.crt, ca/ca.key"

# ========================
# 2. SERVER CERTIFICATE
# ========================
echo "=== Generating Server Certificate ==="

# Generate server private key
openssl genrsa -out server/server.key 2048

# Generate Certificate Signing Request (CSR)
openssl req -new \
  -key server/server.key \
  -sha256 \
  -out server/server.csr \
  -subj "/C=US/ST=California/L=SanFrancisco/O=MyOrg/OU=Engineering/CN=localhost"

# Create extensions file for SAN (Subject Alternative Names)
cat > server/server_ext.cnf <<EOF
authorityKeyIdentifier=keyid,issuer
basicConstraints=CA:FALSE
keyUsage = digitalSignature, keyEncipherment
extendedKeyUsage = serverAuth
subjectAltName = @alt_names

[alt_names]
DNS.1 = localhost
DNS.2 = *.myorg.local
IP.1 = 127.0.0.1
IP.2 = ::1
EOF

# Sign server cert with Root CA
openssl x509 -req \
  -in server/server.csr \
  -CA ca/ca.crt \
  -CAkey ca/ca.key \
  -CAcreateserial \
  -sha256 \
  -days 365 \
  -extfile server/server_ext.cnf \
  -out server/server.crt

echo "Server cert created: server/server.crt (signed by Root CA)"

# ========================
# 3. CLIENT CERTIFICATE (for mutual TLS)
# ========================
echo "=== Generating Client Certificate ==="

# Generate client private key
openssl genrsa -out client/client.key 2048

# Generate CSR
openssl req -new \
  -key client/client.key \
  -sha256 \
  -out client/client.csr \
  -subj "/C=US/ST=California/L=SanFrancisco/O=MyOrg/OU=Engineering/CN=db-client"

# Create extensions file
cat > client/client_ext.cnf <<EOF
authorityKeyIdentifier=keyid,issuer
basicConstraints=CA:FALSE
keyUsage = digitalSignature
extendedKeyUsage = clientAuth
EOF

# Sign client cert with Root CA
openssl x509 -req \
  -in client/client.csr \
  -CA ca/ca.crt \
  -CAkey ca/ca.key \
  -CAcreateserial \
  -sha256 \
  -days 365 \
  -extfile client/client_ext.cnf \
  -out client/client.crt

echo "Client cert created: client/client.crt (signed by Root CA)"

# ========================
# 4. VERIFY THE CHAIN
# ========================
echo ""
echo "=== Verification ==="
echo "--- Server cert chain ---"
openssl verify -CAfile ca/ca.crt server/server.crt
echo "--- Client cert chain ---"
openssl verify -CAfile ca/ca.crt client/client.crt
echo ""
echo "--- Server cert details ---"
openssl x509 -in server/server.crt -text -noout | grep -E "Issuer:|Subject:|Not Before|Not After|Signature Algorithm|Public-Key"
echo ""
echo "=== DONE ==="
echo "Files created:"
echo "  ca/ca.crt, ca/ca.key           — Root CA"
echo "  server/server.crt, server.key  — Server cert (signed by CA)"
echo "  client/client.crt, client.key  — Client cert (signed by CA)"
