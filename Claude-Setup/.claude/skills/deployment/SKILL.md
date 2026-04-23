---
name: deployment
description: Deployment rules and environment conventions for this project
---

# Deployment Rules

## Environment Variables
- Store secrets outside version control
- Use `.env.example` for required variable names only
- Never commit actual secret values

## Deployment Process
- Build artifacts in CI
- Run smoke tests before promoting to production
- Use canary or blue/green deploys when available

## Rollback
- Keep a documented rollback plan for each release
- Test rollback steps in staging before using in production
