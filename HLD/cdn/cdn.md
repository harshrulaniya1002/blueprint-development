# CDN Edge Caching - System Design Guide

A comprehensive guide to understanding Content Delivery Networks and edge caching, used by Netflix, YouTube, Amazon, Cloudflare, and Akamai Technologies to deliver content quickly worldwide.

---

## Table of Contents

1. [What is a CDN?](#what-is-a-cdn)
2. [Why CDN Exists](#why-cdn-exists)
3. [Core CDN Terminology](#core-cdn-terminology)
4. [Basic CDN Architecture](#basic-cdn-architecture)
5. [How CDN Edge Caching Works](#how-cdn-edge-caching-works)
6. [Cache Hit vs Cache Miss](#cache-hit-vs-cache-miss)
7. [Real Example: Netflix](#real-example-netflix)
8. [Types of Content Cached](#types-of-content-cached)
9. [Important CDN Headers](#important-cdn-headers)
10. [TTL (Time To Live)](#ttl-time-to-live)

---

## What is a CDN?

A **CDN (Content Delivery Network)** is a globally distributed network of servers that stores cached copies of content close to users.

Instead of every request going to the main server (origin server), users get content from the nearest edge server.

---

## Why CDN Exists

### Problem Scenario

Imagine your backend server is in **Bangalore, India** but users are from:
- USA
- Europe
- Japan
- Australia

### Without CDN

```
User → Internet → India Server → Response
```

**Issues:**
- High latency
- Slow loading
- More bandwidth usage
- Server overload

### With CDN

```
User → Nearby CDN Edge Server → Response
```

Content is delivered much faster.

---

## Core CDN Terminology

| Term | Meaning |
|------|---------|
| Origin Server | Main backend server storing original content |
| Edge Server | CDN server near users |
| PoP (Point of Presence) | Physical CDN location/datacenter |
| Cache Hit | Data found in edge cache |
| Cache Miss | Data not found, fetched from origin |
| TTL | Cache expiration time |
| Cache Invalidation | Removing stale cache |
| Anycast Routing | Routes user to nearest server |

---

## Basic CDN Architecture

```
                   ┌──────────────┐
                   │ Origin Server│
                   │  (Bangalore) │
                   └──────┬───────┘
                          │
              ┌───────────┼───────────┐
              │           │           │
      ┌───────▼───┐ ┌────▼────┐ ┌────▼────┐
      │ Edge USA  │ │Edge EU  │ │Edge Asia│
      └──────┬────┘ └────┬────┘ └────┬────┘
             │            │           │
         US Users     EU Users    Asia Users
```

---

## How CDN Edge Caching Works

### Step-by-Step Flow

Suppose user requests: `https://example.com/logo.png`

#### Step 1 — User Sends Request
Browser requests: `GET /logo.png`

#### Step 2 — DNS Routes User
CDN DNS sends user to nearest edge location.

**Example:**
- Indian user → Mumbai PoP
- US user → New York PoP

#### Step 3 — Edge Cache Check
Edge server checks: **Is logo.png already cached?**

### CASE A — Cache Hit
If found:
```
Edge Server → User
```
- Very fast response
- No origin involvement

### CASE B — Cache Miss
If not found:
```
Edge → Origin Server → Edge → User
```
- Origin returns content
- Edge stores it in cache
- Then serves user
- Future users get fast responses

---

## Cache Hit vs Cache Miss

### Cache Hit
```
User → Edge Cache → Response
```

**Benefits:**
- Low latency
- Fast loading
- Reduced origin traffic

### Cache Miss
```
User → Edge → Origin → Edge → User
```

Slightly slower initially.

---

## Real Example: Netflix

When millions watch a popular movie:

### Without CDN
```
All requests → Central server → Server dies
```

### With CDN
```
Users → Nearby CDN cache → Traffic distributed globally
```

---

## Types of Content Cached

### Static Content (Ideal for CDN)
- Images
- CSS
- JavaScript
- Videos
- PDFs

### Dynamic Content (Harder)
Examples:
- Personalized dashboard
- Banking data
- Live chat

**Solutions:**
- Dynamic acceleration
- Partial caching
- Edge compute

---

## Important CDN Headers

HTTP caching is controlled using headers.

### Cache-Control
```
Cache-Control: public, max-age=3600
```

**Meaning:**
- Publicly cacheable
- Cache for 1 hour

### Expires
```
Expires: Wed, 25 May 2026 10:00:00 GMT
```

### ETag
Used for validation. If content changed, new ETag is generated.

### Last-Modified
Helps browser check if file changed.

---

## TTL (Time To Live)

TTL determines cache duration.

| Content | TTL |
|---------|-----|
| Logo | 30 days |
| CSS | 1 day |
| API response | 60 seconds |

---

## Cache Invalidation

### Problem
Logo.png is updated, but CDN still has the old version.

### Methods

1. **Purge** - Delete cache immediately
2. **Versioning (Preferred)** - Use versioned filenames
   ```
   logo-v2.png
   app.js?v=2
   ```
   Very common in frontend systems.

---

## CDN Request Routing

CDNs route users intelligently.

### A. Geo Routing
Route to nearest geographical server.

### B. Latency Routing
Route to server with lowest network latency.

### C. Anycast Routing
Same IP globally. Internet routes to nearest PoP automatically.

---

## CDN Caching Strategies

### A. Pull CDN
Most common approach.

**Flow:** Edge pulls from origin when needed

**Examples:**
- Cloudflare
- AWS CloudFront

### B. Push CDN
Origin proactively uploads content to CDN.

**Used for:** Large media distribution

---

## Cache Eviction Policies

When cache is full, which content is removed?

### LRU (Least Recently Used)
Remove oldest unused item. **Most common.**

### LFU (Least Frequently Used)
Remove least accessed content.

### FIFO
Remove oldest inserted item.

---

## Multi-Level CDN Caching

Large CDNs use hierarchy:

```
User
  ↓
Edge Cache
  ↓
Regional Cache
  ↓
Origin
```

**Benefits:**
- Reduces origin traffic further
- Improves scalability

---

## CDN + Browser Cache

Two different caches exist.

| Aspect | Browser Cache | CDN Cache |
|--------|---------------|-----------|
| Location | User device | CDN servers |
| Type | Private | Shared |
| Speed | Very fast | Globally distributed |

Both work together.

---

## Edge Computing

Modern CDNs can execute logic at edge.

**Examples:**
- Authentication
- Rate limiting
- Image resizing
- Geo restrictions

**CDN Services:**
- Cloudflare Workers
- AWS Lambda@Edge

---

## CDN for APIs

APIs can also be cached.

**Example:**
```
GET /products
```
Cached for 60 seconds. Reduces backend load heavily.

---

## Benefits of CDN Edge Caching

### Reduced Latency
Content closer to users.

### Faster Load Time
Improves UX and SEO.

### Reduced Origin Load
Fewer backend requests.

### Better Scalability
Handles traffic spikes.

### High Availability
If one PoP fails, traffic is rerouted.

### Lower Bandwidth Cost
Less data served from origin.

### Security
Many CDNs provide:
- DDoS protection
- WAF (Web Application Firewall)
- TLS termination

---

## CDN Challenges

### Cache Invalidation
Hard problem to solve consistently.

### Dynamic Content
Not easily cacheable.

### Cache Consistency
All edges must stay updated.

### Cache Stampede
Many misses simultaneously hit origin.

**Solutions:**
- Request coalescing
- Stale-while-revalidate

---

## Stale-While-Revalidate

Very important optimization.

**Flow:**
1. Serve old cache immediately
2. Refresh in background

Users get fast responses.

---

## Real-World System Design Example

### Instagram-like Application

#### Without CDN
```
Users worldwide → Central media server
```

**Problems:**
- Slow images/videos
- Huge bandwidth
- High latency

#### With CDN
```
Upload → Origin Storage (S3)
             ↓
         CDN caches
             ↓
       Users get local copies
```

Massively scalable.

---

## CDN in AWS

Typical architecture:

```
Users
   ↓
CloudFront CDN
   ↓
ALB / API Gateway
   ↓
EC2 / Kubernetes
   ↓
Database
```

Static assets often stored in: **S3 + CloudFront**

---

## Important Metrics

### Cache Hit Ratio
```
Cache Hits / Total Requests
```
Higher is better.

### TTFB (Time To First Byte)
CDN reduces TTFB significantly.

---

## Interview Questions You Must Know

### Q1. What happens on cache miss?
Edge fetches from origin, caches response, serves user.

### Q2. How does CDN reduce latency?
By serving content geographically closer.

### Q3. Difference between browser cache and CDN cache?
- Browser = local/private
- CDN = shared/global

### Q4. How do you invalidate stale cache?
- Purge
- TTL expiry
- Versioning

### Q5. What content should NOT be cached?
- Personalized user data
- Banking information
- Real-time stock data

---

## Advanced Concepts

### Cache Key Design
Very important. Bad cache keys cause fragmentation.

**Example:**
```
/products?id=1&utm_source=abc
/products?id=1&utm_source=facebook
```
Creates duplicate cache entries. CDNs normalize cache keys.

### Origin Shield
Additional centralized cache layer protecting origin.

### Request Collapsing
Multiple misses combined into one backend request.

### Signed URLs
Used for protected/private CDN content.

**Examples:**
- Paid videos
- Premium downloads

---

## Best Practices

### Cache static assets aggressively
```
Cache-Control: max-age=31536000
```

### Use file versioning
Avoid stale content.

### Compress assets
Use:
- Brotli
- Gzip

### Use image optimization
WebP/AVIF formats.

### Monitor cache hit ratio
Critical production metric.

---

## Simple End-to-End Example

### First Request
```
User USA → CDN NY Edge (Cache miss)
NY Edge → Bangalore Origin
Response cached
```

### Second Request
```
User USA → CDN NY Edge (Cache hit)
Instant response
```

---

## CDN Providers

Popular CDN providers:
- Cloudflare
- Amazon CloudFront
- Akamai
- Fastly
- Google Cloud CDN

---

## CDN Request Lifecycle

1. User enters URL
2. DNS resolves CDN
3. CDN routes to nearest PoP
4. Edge checks cache
5. Cache hit → serve immediately
6. Cache miss → fetch from origin
7. Store in edge cache
8. Return response to user

---

## DNS and CDN Integration

CDNs heavily depend on DNS.

**Flow:**
```
example.com
   ↓
DNS Query
   ↓
CDN DNS decides nearest edge
   ↓
Returns edge IP
```

**Decision factors:**
- Geography
- ISP
- Network congestion
- Latency
- Server load

---

## Hot vs Cold Content

### Hot Content
Frequently accessed.

**Examples:**
- Viral videos
- Trending images
- Homepage assets

These remain cached.

### Cold Content
Rarely accessed. Eventually evicted.

---

## CDN Consistency Models

### Strong Consistency
All edges updated immediately. Hard and expensive.

### Eventual Consistency
Edges update gradually. Most CDNs use this.

---

## Cache Stampede Detailed Example

Suppose cache expires for `popular-video.mp4`.

Suddenly: 1 million users request simultaneously

### Without Protection
```
1 million requests hit origin → Origin crashes
```

### Solutions

**Request Coalescing**
- Only one request goes to origin
- Others wait

**Stale While Revalidate**
- Serve stale content temporarily
- Refresh in background

**Probabilistic Early Expiration**
- Refresh before actual expiry

---

## CDN Security Features

Modern CDNs are also security layers.

### DDoS Protection
Blocks traffic floods.

### WAF (Web Application Firewall)
Protects against:
- SQL Injection
- XSS
- Bot attacks

### TLS Termination
HTTPS decrypted at edge. Reduces origin CPU load.

### Rate Limiting
Limits abusive requests.

---

## Video Streaming and CDN

Video streaming uses CDN extensively.

### Architecture
```
Uploader
   ↓
Origin Storage
   ↓
Video Transcoding
   ↓
CDN Edge Caches
   ↓
Users stream locally
```

### Adaptive Bitrate Streaming
CDN serves different video qualities based on network speed:
- 240p
- 480p
- 720p
- 1080p

---

## Image CDN Optimization

Modern CDNs optimize images automatically.

**Features:**
- Resize images
- Convert WebP/AVIF
- Compress dynamically
- Device-aware optimization

**Example:**
```
Large desktop image → smaller mobile image
```

---

## CDN and Microservices

In microservice architectures:

```
Users
  ↓
CDN
  ↓
API Gateway
  ↓
Microservices
```

CDN reduces API Gateway load.

---

## API Caching Strategies

### Public APIs
Can cache aggressively.

**Example:**
```
GET /countries
```

### User-Specific APIs
Usually not cacheable.

**Example:**
```
GET /my-profile
```

May require:
- Private caching
- Session-aware caching

---

## Multi-CDN Strategy

Large companies use multiple CDNs.

**Example:**
- Cloudflare
- Akamai
- Fastly

**Benefits:**
- Better redundancy
- Better global performance
- Failover protection
- Reduced vendor dependency

---

## CDN Failover

If one CDN PoP fails: Traffic automatically rerouted

**Techniques:**
- DNS failover
- Anycast rerouting
- Load balancing

---

## Cache Key Optimization

Cache keys determine uniqueness.

**Bad example:**
```
/product?id=1&utm_source=google
/product?id=1&utm_source=facebook
```
Creates duplicate cache entries.

**Better approach:**
Ignore tracking params.

---

## Browser Cache vs Reverse Proxy vs CDN

| Layer | Location | Purpose |
|-------|----------|---------|
| Browser Cache | User device | Fast local access |
| Reverse Proxy Cache | Near backend | Reduce app load |
| CDN Cache | Global edge servers | Reduce global latency |

---

## Reverse Proxy vs CDN

### Reverse Proxy
Usually centralized.

**Examples:**
- Nginx
- Varnish

### CDN
Globally distributed.

**Examples:**
- Cloudflare
- CloudFront

---

## Real Production Example

### Global E-commerce Platform

#### Without CDN
```
All users → Central backend
```

**Problems:**
- Slow product images
- Backend overload
- High cloud bandwidth cost

#### With CDN
```
Users → Nearest CDN Edge → Origin only when needed
```

**Result:**
- Faster product pages
- Reduced backend traffic
- Better scaling during sales

---

## CDN Metrics to Monitor

### Cache Hit Ratio
Higher is better.

### Origin Offload
Measures traffic reduction.

### Latency
Time taken for response.

### Error Rate
Monitor failed edge requests.

### Bandwidth Savings
Measures CDN efficiency.

---

## Common CDN Interview Problems

### Design YouTube CDN
Needs:
- Video chunking
- Edge replication
- Streaming optimization
- Adaptive bitrate

### Design Instagram Media Delivery
Needs:
- Image optimization
- Global caching
- Signed URLs
- Hot content distribution

### Design Netflix CDN
Needs:
- Massive edge storage
- Regional replication
- ISP integration
- Predictive caching

---

## Final Architecture Summary

```
        ┌────────────────────┐
        │   Origin Server    │
        │  App + Database    │
        └─────────┬──────────┘
                  │
        ┌─────────▼──────────┐
        │   Regional Cache   │
        └─────────┬──────────┘
                  │
      ┌───────────┼───────────┐
      │           │           │
┌─────▼─────┐ ┌──▼──────┐ ┌──▼──────┐
│ Edge USA  │ │Edge IND │ │Edge EUR │
└─────┬─────┘ └──┬──────┘ └──┬──────┘
      │          │           │
    Users     Users        Users
   Nearby     Nearby       Nearby
```

---

## Key Takeaways

CDN Edge Caching is one of the foundational technologies behind modern internet scalability.

### It enables:
- ✅ Fast global delivery
- ✅ Reduced backend load
- ✅ Better scalability
- ✅ Lower latency
- ✅ Improved availability
- ✅ Better user experience

### Core Flow:
```
User → Edge Cache → Origin (if needed)
```

**CDN Edge Caching is essential for:**
- System Design Interviews
- Distributed Systems
- Large Scale Architecture
- High Performance Backend Systems

Virtually every large-scale platform today depends heavily on CDN infrastructure.

---

**Last Updated:** May 23, 2026
