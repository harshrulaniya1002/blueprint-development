---
name: api-conventions
description: REST API design rules and error response shapes for this project
---

# API Conventions

## Endpoint Naming
- Resources are plural nouns: `/users`, `/orders`, `/products`
- Nested routes max 2 levels: `/users/:id/orders`
- Actions use verbs: `/users/:id/activate` (POST)

## Response Shape
```json
{
  "data": { ... },
  "meta": { "page": 1, "total": 100 },
  "error": null
}
```

## Error Shape
```json
{
  "error": {
    "code": "VALIDATION_ERROR",
    "message": "Human-readable description",
    "details": [...]
  }
}
```

## Auth
- All endpoints require `Authorization: Bearer <token>` except `/auth/*`
- Return 401 for missing/invalid token, 403 for insufficient permissions
