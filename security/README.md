# fibs — TLS, Cryptography & Authentication Playground

A hands-on Go codebase for learning and experimenting with TLS configuration, certificate management, mutual TLS, FIPS 140-3 compliance, JWT authentication, and modern cryptographic algorithms.

---

## Repository Structure

```
fibs/
├── main_with_TLS.go          # HTTPS server — TLS with RSA certs, custom cipher suites
├── main_without_TLS.go       # Plain HTTP server — no TLS
├── main_no_tls.go            # Minimal Go HTTP server baseline
├── main_postgres_tls.go      # PostgreSQL over mTLS — FIPS 140-3 compliant config
│
├── authentication/           # Full JWT auth service (Go)
│   ├── backend/
│   │   ├── handlers/         # Login, protected route, token refresh
│   │   ├── middleware/        # JWT validation middleware
│   │   ├── routes/            # Route wiring
│   │   └── token/             # JWT sign/verify, access + refresh token logic
│   └── frontend/
│       ├── login.html         # Login page
│       └── profile.html       # Protected profile page
│
├── certs/                    # Certificate generation scripts and outputs
│   ├── commands.txt           # OpenSSL commands reference
│   ├── rsa_certs/             # Self-signed RSA certificates
│   ├── ecdsa_certs/           # ECDSA certificates (P-256 / P-384)
│   ├── ed25519_certs/         # Ed25519 certificates
│   └── certs_chain/           # CA-signed certificate chain (CA → server + client)
│       ├── ca/                # Root CA cert and key
│       ├── server/            # Server cert signed by CA
│       └── client/            # Client cert for mutual TLS
│
└── docs/                     # HTML reference guides (open in browser)
    ├── crypto-algorithms-explainer.html   # Symmetric, Asymmetric, Signatures, ECC, DH, TLS, PQC
    ├── tls13_flow.html                    # TLS 1.3 full handshake end-to-end flow
    ├── fips_tls_explainer.html            # FIPS 140-3 with PostgreSQL + Go walkthrough
    └── jwt-explainer.html                 # JWT structure, HMAC/RSA signing, refresh tokens
```

---

## Topics Covered

### 1. TLS Server Configurations

| File | Description |
|---|---|
| `main_with_TLS.go` | HTTPS with RSA certs; shows `tls.Config` with explicit cipher suites |
| `main_without_TLS.go` | Plain HTTP — baseline before adding TLS |
| `main_no_tls.go` | Minimal HTTP server |
| `main_postgres_tls.go` | mTLS connection to PostgreSQL; FIPS 140-3 cipher suite selection |

**Key Go concepts:**
```go
tlsConfig := &tls.Config{
    MinVersion:   tls.VersionTLS12,
    CipherSuites: []uint16{
        tls.TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256,
        tls.TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384,
    },
    RootCAs:      caCertPool,      // verify server cert
    Certificates: []tls.Certificate{clientCert}, // mutual TLS
}
```

To run in FIPS-only mode:
```bash
GODEBUG=fips140=only go run main_postgres_tls.go
# or build with the FIPS module
GOFIPS140=latest go build -o app main_postgres_tls.go
```

---

### 2. Certificate Types & Generation

Three key types of certificates are explored, each with different trade-offs:

| Type | Key Algorithm | Curve / Params | Size | Use Cases |
|---|---|---|---|---|
| RSA | RSA | 2048–4096 bit | Large | Legacy TLS, broad compatibility |
| ECDSA | Elliptic Curve DSA | P-256, P-384 | Compact | Modern TLS, FIPS-approved |
| Ed25519 | EdDSA | Curve25519 | Smallest | SSH, TLS 1.3, no nonce risk |

**Certificate chain (CA → server/client):**
- Root CA signs both the server cert and the client cert
- Server presents cert during TLS handshake → client verifies against CA
- Mutual TLS (mTLS): client also presents its cert → server verifies

OpenSSL commands are documented in `certs/commands.txt`.

---

### 3. Symmetric Encryption

Same key encrypts and decrypts. Fast — used for bulk data (TLS record layer).

| Algorithm | Mode | Auth | Notes |
|---|---|---|---|
| AES-128/256-GCM | AEAD | ✅ | TLS 1.3 default — encrypts + authenticates |
| ChaCha20-Poly1305 | Stream+MAC | ✅ | Mobile / no AES-NI hardware |
| AES-CBC | Block | ❌ | Legacy TLS 1.2 — vulnerable to padding oracle |
| 3DES / DES / RC4 | — | ❌ | Broken / deprecated — never use |

**AES-GCM** is an AEAD cipher — one pass gives confidentiality + integrity + authenticity. TLS 1.3 mandates AEAD-only cipher suites.

---

### 4. Asymmetric (Public-Key) Encryption

A key pair: **public key** (shared freely) + **private key** (kept secret).

- Solves key distribution — no pre-shared secret needed
- In TLS 1.3, RSA/ECDSA are used **only for authentication** (certificate signatures), not for key exchange
- Key exchange is handled by ephemeral ECDHE

| Algorithm | Based On | Use |
|---|---|---|
| RSA-2048 / 4096 | Integer factorization | TLS certs, email (S/MIME) |
| RSA-OAEP | Integer factorization | Key encapsulation, encrypting session keys |
| ECIES | ECDLP | Mobile encryption, Ethereum |

---

### 5. Digital Signature Algorithms

Signatures prove **who sent the data** and that it **wasn't tampered with**.

| Algorithm | Key Size | Notable Property | Use Cases |
|---|---|---|---|
| RSA-PSS | 2048–4096 bit | Probabilistic padding | TLS certs, JWT PS256 |
| ECDSA P-256 | 256 bit | Fast, compact sig | TLS 1.3 certs, JWT ES256, code signing |
| Ed25519 | 255 bit | **Deterministic** — no nonce risk | SSH, TLS 1.3, JWT EdDSA, WireGuard |
| Ed448 | 448 bit | Highest classical security | Long-lived keys |
| DSA | 1024–3072 | Legacy | Deprecated |

**Ed25519 vs ECDSA:** ECDSA requires a random nonce per signature — reusing it leaks the private key (the PS3 hack). Ed25519 is deterministic, faster, and produces smaller signatures.

---

### 6. Elliptic Curve Cryptography (ECC)

ECC achieves the same security as RSA with dramatically smaller keys.

| Security Level | RSA Key | ECC Key |
|---|---|---|
| 128-bit | 3072 bit | 256 bit |
| 192-bit | 7680 bit | 384 bit |
| 256-bit | 15360 bit | 521 bit |

**Named curves used in this project:**

| Curve | Standard | Used For |
|---|---|---|
| P-256 (secp256r1) | NIST | TLS certs, FIPS-approved ECDSA/ECDHE |
| P-384 (secp384r1) | NIST | High-security TLS, NSA Suite B |
| Curve25519 | IETF RFC 7748 | X25519 key exchange in TLS 1.3 |
| Edwards25519 | IETF RFC 8032 | Ed25519 signatures |

---

### 7. Diffie-Hellman Key Exchange

DH lets two parties compute a **shared secret over an insecure channel** — nothing secret is transmitted.

```
Alice: A = g^a mod p  →  sends A
Bob:   B = g^b mod p  →  sends B

Alice: S = B^a mod p
Bob:   S = A^b mod p
Both get S = g^(ab) mod p — never transmitted
```

**Variants:**

| Variant | Forward Secrecy | TLS 1.3 |
|---|---|---|
| Static DH / ECDH | ❌ | Removed |
| DHE (ephemeral) | ✅ | TLS 1.2 only |
| ECDHE (ephemeral EC) | ✅ | **Mandatory** |

**ECDHE in TLS 1.3** uses X25519 or P-256. Each session generates a fresh key pair — compromising the server's long-term key does not decrypt past sessions (**forward secrecy**).

---

### 8. TLS 1.3 Handshake

TLS 1.3 completes in **1 round-trip** (vs 2 in TLS 1.2).

```
Client → ServerHello (KeyShare: X25519 public key)
Server → ServerHello (KeyShare) + Certificate + CertificateVerify + Finished
         ↑ all encrypted with handshake keys derived from ECDHE shared secret
Client → Finished
         ↑ both sides derive application traffic keys via HKDF
─── Application data (AES-GCM / ChaCha20-Poly1305) ───
```

**What each algorithm does:**

| Role | Algorithm |
|---|---|
| Key Exchange | ECDHE — X25519 or P-256 |
| Authentication | ECDSA, RSA-PSS, or Ed25519 (in certificate) |
| Key Derivation | HKDF-SHA256 / SHA384 |
| Record Encryption | AES-128/256-GCM or ChaCha20-Poly1305 (AEAD) |

**Removed in TLS 1.3:** RSA key exchange, static DH, RC4, 3DES, CBC-mode ciphers, SHA-1 in certs, compression.

---

### 9. FIPS 140-3

FIPS 140-3 restricts which algorithms a system may use for federal/government workloads.

| Category | FIPS Approved | FIPS Rejected |
|---|---|---|
| Symmetric | AES-128, AES-256 (GCM, CBC) | RC4, DES, 3DES, ChaCha20 |
| Asymmetric | RSA (≥2048), ECDSA P-256/P-384 | RSA <2048, Ed25519 |
| Hash | SHA-2 family (256/384/512) | MD5, SHA-1 |
| Key Exchange | ECDHE P-256/P-384, DHE ≥2048 | X25519 (Curve25519) |
| TLS | 1.2 minimum, 1.3 supported | TLS 1.0, 1.1, SSL |

Go FIPS mode (`GODEBUG=fips140=only`) automatically rejects non-approved algorithms at runtime.

See `docs/fips_tls_explainer.html` for the full PostgreSQL + Go walkthrough.

---

### 10. JWT Authentication

The `authentication/` service implements stateless auth with access and refresh tokens.

**Token flow:**
```
POST /login  →  returns access_token (15 min) + refresh_token (7 days)
               both set as HttpOnly Secure SameSite=Strict cookies

GET /profile  →  JWT middleware validates access_token
POST /refresh →  validates refresh_token, issues new access_token
```

**JWT structure:**
```
header.payload.signature
   ↓         ↓          ↓
base64    base64    HMAC-SHA256(header+payload, secret)
                   or RSA-PSS / ECDSA / Ed25519
```

The payload is **not encrypted** — never put passwords or secrets in claims. The signature is what makes it tamper-proof.

**Supported algorithms:** HS256 (HMAC), RS256/PS256 (RSA), ES256 (ECDSA), EdDSA (Ed25519)

---

### 11. Post-Quantum Cryptography (PQC)

Quantum computers running **Shor's algorithm** break all RSA, DH, ECDH, and ECDSA. NIST finalized four standards in 2024:

| Standard | Former Name | Type | Security Basis |
|---|---|---|---|
| FIPS 203 — ML-KEM | CRYSTALS-Kyber | Key exchange (KEM) | Lattice (MLWE) |
| FIPS 204 — ML-DSA | CRYSTALS-Dilithium | Signature | Lattice (MLWE) |
| FIPS 205 — SLH-DSA | SPHINCS+ | Signature | Hash-based (SHA-256) |
| FIPS 206 — FN-DSA | Falcon | Signature | NTRU lattice |

**Hybrid key exchange** (used in production today):
```
X25519MLKEM768 = X25519 shared secret ⊕ ML-KEM-768 shared secret
```
Secure if either algorithm remains unbroken. Supported by Chrome, Cloudflare, AWS, and Go 1.23+.

> ⚠️ **"Harvest now, decrypt later":** Adversaries archive encrypted traffic today to decrypt once quantum computers mature. Migrate to hybrid PQC for long-lived sensitive data now.

---

## Reference Docs

Open these in a browser — no server required:

| File | Contents |
|---|---|
| `docs/crypto-algorithms-explainer.html` | Complete reference: Symmetric, Asymmetric, Signatures, ECC, DH, TLS 1.3, PQC, comparison tables |
| `docs/tls13_flow.html` | Animated TLS 1.3 handshake flow — message-by-message |
| `docs/fips_tls_explainer.html` | FIPS 140-3 with PostgreSQL + Go — cipher config, Wireshark verification |
| `docs/jwt-explainer.html` | JWT structure, HMAC vs RSA signing, refresh token flow |

### Previews

**Asymmetric Encryption — `crypto-algorithms-explainer.html`**
![Asymmetric encryption — algorithm table with RSA, ECIES, ElGamal](docs/Screenshot%202026-08-02%20124013.png)

**FIPS 140-3 & TLS with PostgreSQL + Go — `fips_tls_explainer.html`**
![FIPS 140-3 explainer — approved vs rejected algorithms, PostgreSQL TLS config](docs/Screenshot%202026-08-02%20124030.png)

**How JWT Works — `jwt-explainer.html`**
![JWT structure — header, payload, signature with decoded views](docs/Screenshot%202026-08-02%20124042.png)

**TLS 1.3 End-to-End Flow — `tls13_flow.html`**
![TLS 1.3 handshake — key exchange, authentication, performance, simplified design](docs/Screenshot%202026-08-02%20124103.png)

---

## Quick Start

**Run the plain HTTPS server (RSA certs):**
```bash
cd fibs
go run main_with_TLS.go
# Listens on :9999 — test with: curl -k https://localhost:9999/test-connection
```

**Run the JWT auth service:**
```bash
cd authentication
JWT_SECRET=your-secret go run main.go
# POST http://localhost:8080/login  {"username":"user","password":"pass"}
# GET  http://localhost:8080/profile  (requires access_token cookie)
```

**Run PostgreSQL mTLS (FIPS mode):**
```bash
GODEBUG=fips140=only go run main_postgres_tls.go
```

**Generate certificates (from certs/commands.txt):**
```bash
# RSA self-signed
openssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -days 365 -nodes

# ECDSA P-256
openssl req -x509 -newkey ec -pkeyopt ec_paramgen_curve:P-256 -keyout key.pem -out cert.pem -days 365 -nodes

# Ed25519
openssl req -x509 -newkey ed25519 -keyout key.pem -out cert.pem -days 365 -nodes

# CA-signed chain — see certs/certs_chain/generate_ca_signed_certs.sh
```

---

## Key Takeaways

- **Use ECDHE** (X25519 or P-256) for key exchange — never static DH or RSA key transport
- **Use AES-256-GCM or ChaCha20-Poly1305** for symmetric encryption — always AEAD
- **Prefer Ed25519** over ECDSA for signatures — deterministic, faster, no nonce risk
- **TLS 1.3 is mandatory** for new systems — 1 RTT, AEAD-only, forward secrecy built-in
- **FIPS 140-3** requires NIST curves (P-256/P-384) and rejects X25519, Ed25519, ChaCha20
- **JWT payloads are not secret** — sign them, never put sensitive data in claims
- **Start hybrid PQC migration** — X25519+ML-KEM-768 is deployable today
