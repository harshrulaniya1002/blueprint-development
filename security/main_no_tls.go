package main

// WARNING: This file connects to PostgreSQL WITHOUT TLS.
// All data — including credentials and query results — travels in plaintext.
// Visible to anyone who can capture network traffic (Wireshark, tcpdump, etc.).
//
// Safe ONLY for:
//   - Local development on a single machine (loopback, never leaves the OS)
//   - Isolated environments with no network exposure
//
// NOT safe for:
//   - Any production or staging environment
//   - Connections that cross a network (even internal/corporate)
//   - Any system subject to FIPS 140-3, PCI-DSS, HIPAA, or SOC 2

import (
	"database/sql"
	"fmt"
	"log"

	_ "github.com/jackc/pgx/v5/stdlib"
)

func main() {
	// sslmode=disable tells PostgreSQL: do not even attempt TLS
	db, err := sql.Open("pgx", "host=localhost port=5432 dbname=mydb user=myuser password=CHANGEME sslmode=disable")
	if err != nil {
		log.Fatalf("Failed to open DB: %v", err)
	}
	defer db.Close()

	var version string
	if err := db.QueryRow("SELECT version()").Scan(&version); err != nil {
		log.Fatalf("Query failed: %v", err)
	}
	fmt.Println("Connected WITHOUT TLS (plaintext)")
	fmt.Println("Server version:", version)

	// Confirm TLS is not in use
	var sslInUse bool
	err = db.QueryRow(`SELECT ssl FROM pg_stat_ssl WHERE pid = pg_backend_pid()`).Scan(&sslInUse)
	if err != nil {
		log.Printf("Could not check SSL status: %v", err)
	} else if sslInUse {
		fmt.Println("WARNING: SSL is in use despite sslmode=disable")
	} else {
		fmt.Println("SSL in use: false — connection is plaintext")
	}
}
