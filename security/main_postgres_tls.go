package main

import (
	"crypto/tls"
	"crypto/x509"
	"database/sql"
	"fmt"
	"log"
	"os"

	"github.com/jackc/pgx/v5"
	"github.com/jackc/pgx/v5/stdlib"
)

// To run in FIPS 140-3 mode:
//   GODEBUG=fips140=only go run main_postgres_tls.go
//
// Or build with FIPS module:
//   GOFIPS140=latest go build -o app main_postgres_tls.go

func main3() {
	// --- Load CA certificate to verify PostgreSQL server ---
	caCert, err := os.ReadFile("./certs/certs_chain/ca/ca.crt")
	if err != nil {
		log.Fatalf("Failed to read CA cert: %v", err)
	}
	caCertPool := x509.NewCertPool()
	if !caCertPool.AppendCertsFromPEM(caCert) {
		log.Fatal("Failed to add CA cert to pool")
	}

	// --- Optional: Load client certificate for mutual TLS ---
	clientCert, err := tls.LoadX509KeyPair(
		"./certs/certs_chain/client/client.crt",
		"./certs/certs_chain/client/client.key",
	)
	if err != nil {
		log.Fatalf("Failed to load client cert: %v", err)
	}

	// --- TLS config (FIPS-compliant when GODEBUG=fips140=only) ---
	//
	// When running in FIPS mode, Go automatically:
	//   - Rejects non-FIPS cipher suites (ChaCha20, etc.)
	//   - Rejects non-FIPS curves (x25519)
	//   - Rejects non-FIPS signature algorithms (Ed25519)
	//   - Enforces minimum key sizes (RSA >= 2048)
	//
	// The cipher suites below are all FIPS-approved. In FIPS mode,
	// any non-approved suite would be silently ignored anyway.
	tlsConfig := &tls.Config{
		MinVersion:   tls.VersionTLS12,
		MaxVersion:   tls.VersionTLS12,
		ServerName:   "localhost", // must match the CN/SAN in the server certificate
		RootCAs:      caCertPool,
		Certificates: []tls.Certificate{clientCert}, // For mutual TLS

		CipherSuites: []uint16{
			// TLS 1.2 FIPS-approved cipher suites
			tls.TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256,
			tls.TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384,
			tls.TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256,
			tls.TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384,
			// TLS 1.3 suites are not configurable — Go auto-selects
			// TLS_AES_128_GCM_SHA256 and TLS_AES_256_GCM_SHA384
			// (both FIPS-approved). ChaCha20 is excluded in FIPS mode.
		},
		
		CurvePreferences: []tls.CurveID{
			tls.CurveP256, // FIPS-approved
			tls.CurveP384, // FIPS-approved
			// tls.X25519 — NOT FIPS-approved, excluded
		},
	}

	// --- Connect to PostgreSQL with TLS ---
	connConfig, err := pgx.ParseConfig(
		"host=localhost port=5432 dbname=mydb user=myuser password=CHANGEME sslmode=require",
	)
	if err != nil {
		log.Fatalf("Failed to parse config: %v", err)
	}
	connConfig.TLSConfig = tlsConfig
	// Prevent silent fallback to plaintext if TLS negotiation fails
	connConfig.Fallbacks = nil

	// Register pgx driver with database/sql
	connStr := stdlib.RegisterConnConfig(connConfig)
	db, err := sql.Open("pgx", connStr)
	if err != nil {
		log.Fatalf("Failed to open DB: %v", err)
	}
	defer db.Close()

	// Verify connection
	var version string
	err = db.QueryRow("SELECT version()").Scan(&version)
	if err != nil {
		log.Fatalf("Query failed: %v", err)
	}
	fmt.Println("Connected to PostgreSQL with TLS!")
	fmt.Println("Server version:", version)

	// Verify TLS is active — pg_stat_ssl is built-in, no extension needed
	var sslInUse bool
	var tlsVersion, cipher sql.NullString
	err = db.QueryRow(`
		SELECT ssl, version, cipher
		FROM pg_stat_ssl
		WHERE pid = pg_backend_pid()
	`).Scan(&sslInUse, &tlsVersion, &cipher)
	if err != nil {
		log.Printf("Could not check SSL status: %v", err)
	} else {
		fmt.Printf("SSL in use: %v\n", sslInUse)
		fmt.Printf("TLS version: %s\n", tlsVersion.String)
		fmt.Printf("Cipher suite: %s\n", cipher.String)
	}
}
