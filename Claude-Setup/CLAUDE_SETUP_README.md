# 🧠 Production-Grade Claude Code Setup

> The definitive guide to configuring Claude Code for serious engineering work — covering CLAUDE.md, skills, hooks, MCP servers, subagents, slash commands, and project structure.

---

## 📚 Table of Contents

1. [Overview](#overview)
2. [Project File Structure](#project-file-structure)
3. [CLAUDE.md — The Core Memory File](#claudemd--the-core-memory-file)
4. [CLAUDE.local.md — Personal Overrides](#claudelocalmd--personal-overrides)
5. [Skills — On-Demand Domain Knowledge](#skills--on-demand-domain-knowledge)
6. [Hooks — Deterministic Guardrails](#hooks--deterministic-guardrails)
7. [Slash Commands — Reusable Workflows](#slash-commands--reusable-workflows)
8. [Subagents — Specialized AI Workers](#subagents--specialized-ai-workers)
9. [MCP Servers — External Tool Integrations](#mcp-servers--external-tool-integrations)
10. [settings.json — Permissions & Behaviour](#settingsjson--permissions--behaviour)
11. [Global Setup (~/.claude/)](#global-setup-claude)
12. [Planning Workflow](#planning-workflow)
13. [Session Best Practices](#session-best-practices)
14. [Anti-Patterns to Avoid](#anti-patterns-to-avoid)
15. [Recommended MCP Stack](#recommended-mcp-stack)

---

## Overview

Claude Code is stateless — it knows nothing about your project at the start of every session unless you explicitly give it that context. This setup turns Claude Code from a smart autocomplete into a production-grade engineering partner by investing in four areas:

- **Persistent context** via CLAUDE.md (what Claude always knows)
- **On-demand knowledge** via skills (what Claude loads when relevant)
- **Enforced behaviour** via hooks (rules that can't be ignored)
- **External capabilities** via MCP servers (real-world tools)

---

## Project File Structure

```
my-project/
├── CLAUDE.md                    # ✅ Shared team context — commit to git
├── CLAUDE.local.md              # 🔒 Personal overrides — add to .gitignore
├── .mcp.json                    # ✅ Project MCP servers — commit to git
├── .claude/
│   ├── settings.json            # ✅ Permissions & model config
│   ├── commands/                # ✅ Custom slash commands
│   │   ├── plan.md
│   │   ├── review.md
│   │   └── spec.md
│   ├── agents/                  # ✅ Subagent definitions
│   │   ├── reviewer.md
│   │   └── db-agent.md
│   ├── skills/                  # ✅ On-demand domain knowledge
│   │   ├── api-conventions/
│   │   │   └── SKILL.md
│   │   └── deployment/
│   │       └── SKILL.md
│   └── rules/                   # ✅ Auto-loaded per-directory rules
│       ├── markdown-docs.md
│       └── security.md
├── docs/
│   ├── SPEC.md                  # Architecture decisions & feature specs
│   └── PLAN.md                  # Current working plan (updated each session)
└── .gitignore
    # Add: CLAUDE.local.md
```

---

## How to apply this setup to a local project

**Option 1: Clone the scaffold and customize (Recommended)**

1. Clone the scaffold as a template for your new project.
   ```bash
   git clone /Users/harshvardhan/Documents/development/Dev/Claude-Setup /path/to/your-new-project
   cd /path/to/your-new-project
   ```

2. Start fresh with your project's own git history.
   ```bash
   rm -rf .git
   git init
   git add .
   git commit -m "Initial commit: Claude Code scaffold"
   ```

3. Customize `CLAUDE.md` for your project:
   - Update the overview with your project's purpose
   - Set stack details (runtime, framework, database, testing tools)
   - Add essential commands specific to your project
   - Add your team's code style and workflow rules

4. Update `.claude/settings.json`:
   - Adjust permissions to match your repo structure
   - Configure hooks for your code formatters and linters
   - Set the model preference

5. Personalize `CLAUDE.local.md`:
   - Add your personal preferences (keep this out of version control)
   - Ensure `.gitignore` includes `CLAUDE.local.md`

6. Add project-specific configurations:
   - Update `.mcp.json` with MCP servers for your stack
   - Add domain-specific skills under `.claude/skills/`
   - Add project rules under `.claude/rules/`

7. Optional: Set up global Claude config at `~/.claude/`:
   - `~/.claude/CLAUDE.md` — shared preferences across all projects
   - `~/.claude/hooks/*` — global hook scripts

---

## CLAUDE.md — The Core Memory File

This is the single most important file in your setup. Claude reads it at the start of **every session**. Keep it under 200 lines — beyond that, instruction-following degrades uniformly.

### Golden Rule

> For every line, ask: *"Would Claude make a mistake without this?"* If not, cut it.

### Template

```markdown
# Project: [Name]

## Overview
[2-3 sentences: what the project does and why it exists]

## Stack
- Runtime: Node.js 20 / Python 3.12 / etc.
- Framework: Next.js 14 / FastAPI / etc.
- DB: PostgreSQL via Prisma / etc.
- Testing: Vitest + Playwright

## Project Structure
- `src/app/` — Next.js App Router pages
- `src/components/` — Shared React components
- `src/lib/` — Business logic and utilities
- `src/server/` — API routes and tRPC routers
- `prisma/` — Schema and migrations

## Essential Commands
```bash
pnpm dev          # Start dev server
pnpm test         # Run unit tests
pnpm test:e2e     # Run Playwright tests
pnpm build        # Production build
pnpm typecheck    # TypeScript check (always run before PRs)
pnpm lint         # ESLint
```

## Code Style
- Use ES modules (import/export), not CommonJS
- Async/await over raw Promises
- Functional components only (no class components)
- Zod for all runtime validation
- Never use `any` in TypeScript

## Workflow Rules
- Always run `pnpm typecheck && pnpm test` before marking a task done
- Never commit secrets or API keys
- Create a separate commit per logical change
- IMPORTANT: Never use `--force` push to main

## Additional Context
See @docs/SPEC.md for architecture decisions
See @docs/PLAN.md for the current working plan
```

### CLAUDE.md Placement Options

| Location | Scope | Commit? |
|---|---|---|
| `~/.claude/CLAUDE.md` | All sessions globally | Personal |
| `./CLAUDE.md` | Project root — team-wide | ✅ Yes |
| `./CLAUDE.local.md` | Project root — personal | ❌ .gitignore |
| `./src/CLAUDE.md` | Subdirectory context | ✅ Yes |

Use `@path/to/file` syntax inside CLAUDE.md to import other files:
```markdown
See @README.md for project overview
See @package.json for available npm commands
Deployment rules: @docs/deploy.md
```

---

## CLAUDE.local.md — Personal Overrides

Personal preferences that apply to your sessions only. Add to `.gitignore`.

```markdown
# My Personal Preferences

## Output Style
- Always explain your reasoning before writing code
- Show me a plan and wait for approval before making changes > 50 lines

## My Workflow
- I prefer functional over object-oriented patterns
- Always suggest tests when writing new functions
- When context gets long, remind me to /clear
```

---

## Skills — On-Demand Domain Knowledge

Skills are markdown files that Claude loads **only when relevant** — keeping CLAUDE.md lean. Put them in `.claude/skills/<name>/SKILL.md`.

### When to use skills vs CLAUDE.md

| Use CLAUDE.md | Use a Skill |
|---|---|
| Applies to every session | Applies to specific workflows |
| Basic stack & commands | Deep API conventions |
| Core code style | Deployment runbooks |
| Team-wide rules | Third-party integration guides |

### Example Skill: `.claude/skills/api-conventions/SKILL.md`

```markdown
---
name: api-conventions
description: REST API design rules and error response shapes for this project
---

# API Conventions

## Endpoint Naming
- Resources are plural nouns: /users, /orders, /products
- Nested routes max 2 levels: /users/:id/orders
- Actions use verbs: /users/:id/activate (POST)

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
- All endpoints require `Authorization: Bearer <token>` except /auth/*
- Return 401 for missing/invalid token, 403 for insufficient permissions
```

### Importing skills in CLAUDE.md

```markdown
For API work, see @.claude/skills/api-conventions/SKILL.md
For deployments, see @.claude/skills/deployment/SKILL.md
```

---

## Hooks — Deterministic Guardrails

Hooks are shell scripts that run on specific Claude Code events. **Unlike CLAUDE.md instructions, hooks cannot be ignored.** Exit code 2 blocks the action entirely.

Hook events: `PreToolUse`, `PostToolUse`, `Stop`, `SubagentStop`, `Notification`

### Configuration: `.claude/settings.json`

```json
{
  "hooks": {
    "PreToolUse": [
      {
        "matcher": "Bash",
        "hooks": [
          {
            "type": "command",
            "command": "~/.claude/hooks/block-dangerous-commands.sh"
          }
        ]
      }
    ],
    "PostToolUse": [
      {
        "matcher": "Write|Edit|MultiEdit",
        "hooks": [
          {
            "type": "command",
            "command": "~/.claude/hooks/auto-format.sh"
          }
        ]
      }
    ]
  }
}
```

### Hook: Block Dangerous Commands

`~/.claude/hooks/block-dangerous-commands.sh`
```bash
#!/bin/bash
# Reads the tool input from stdin as JSON
INPUT=$(cat)
COMMAND=$(echo "$INPUT" | jq -r '.command // ""')

# Block destructive patterns
if echo "$COMMAND" | grep -qE 'rm -rf /|DROP TABLE|DELETE FROM .* WHERE 1|git push --force'; then
  echo "⛔ Blocked: potentially destructive command detected. Confirm manually." >&2
  exit 2
fi

exit 0
```

### Hook: Auto-format after edits

`~/.claude/hooks/auto-format.sh`
```bash
#!/bin/bash
INPUT=$(cat)
FILE=$(echo "$INPUT" | jq -r '.path // ""')

# Only format JS/TS files
if echo "$FILE" | grep -qE '\.(ts|tsx|js|jsx)$'; then
  npx prettier --write "$FILE" 2>/dev/null
fi

exit 0
```

### Hook: Audit log

`~/.claude/hooks/audit-log.sh`
```bash
#!/bin/bash
INPUT=$(cat)
echo "[$(date -u +"%Y-%m-%dT%H:%M:%SZ")] $INPUT" >> ~/.claude/audit.log
exit 0
```

---

## Slash Commands — Reusable Workflows

Create `.claude/commands/<name>.md` — invoke with `/<name>` in any session.

### `/plan` — Structured Planning Mode

`.claude/commands/plan.md`
```markdown
Create a phased implementation plan for: $ARGUMENTS

Structure the plan as:
1. **Phase 1 — Foundation**: Core data models, types, interfaces
2. **Phase 2 — Core Logic**: Business logic, API handlers
3. **Phase 3 — UI**: Components and pages
4. **Phase 4 — Tests**: Unit, integration, e2e
5. **Phase 5 — Hardening**: Error handling, edge cases, performance

For each phase:
- List the specific files to create or modify
- Identify acceptance criteria and tests
- Flag any architectural decisions needed

Write the plan to docs/PLAN.md and wait for my approval before coding.
```

### `/spec` — Interview-Driven Spec Writing

`.claude/commands/spec.md`
```markdown
I want to build: $ARGUMENTS

Interview me thoroughly using the AskUserQuestion tool. Cover:
- Core user flows and edge cases  
- Data models and relationships
- API design
- Performance requirements
- Security constraints
- What can be deferred to v2

Don't ask obvious questions. Keep interviewing until we've covered everything.
Then write a complete spec to docs/SPEC.md.
Once approved, start a fresh session to execute it.
```

### `/review` — Staff Engineer Code Review

`.claude/commands/review.md`
```markdown
Review the current changes (or $ARGUMENTS) like a senior staff engineer.

Check for:
- Correctness and edge case handling
- Security vulnerabilities (injection, auth bypass, data exposure)
- Performance issues (N+1 queries, missing indexes, blocking I/O)
- TypeScript type safety
- Test coverage gaps
- Naming clarity and code organisation

Be specific. Cite line numbers. Don't just say "looks good." 
Challenge every shortcut.
```

---

## Subagents — Specialized AI Workers

Subagents are Claude instances with scoped tools and focused system prompts. Define them in `.claude/agents/<name>.md`.

### Reviewer Agent

`.claude/agents/reviewer.md`
```markdown
---
name: reviewer
description: Reviews code changes from a fresh, unbiased context
model: claude-sonnet-4-6
tools: Read, Glob, Grep
---

You are a senior engineer reviewing a pull request. You have no knowledge of
the implementation decisions made — you only see the code.

Review for: correctness, security, performance, type safety, test coverage.
Be specific. Reference file paths and line numbers. Don't rubber-stamp anything.
```

### Database Agent

`.claude/agents/db-agent.md`
```markdown
---
name: db-agent
description: Handles database queries and schema changes
model: claude-sonnet-4-6
tools: Bash, Read, Write
---

You manage the database layer. You have access to the Prisma schema and can
run migrations. You NEVER run destructive operations on production.

Always:
- Run queries in a transaction when modifying data
- Validate migrations in development first
- Check for missing indexes on foreign keys
```

---

## MCP Servers — External Tool Integrations

MCP servers give Claude real-world tools beyond code editing.

### Project-scoped: `.mcp.json`

```json
{
  "mcpServers": {
    "filesystem": {
      "command": "npx",
      "args": ["-y", "@modelcontextprotocol/server-filesystem", "./"],
      "env": {}
    },
    "postgres": {
      "command": "npx",
      "args": ["-y", "@modelcontextprotocol/server-postgres"],
      "env": {
        "POSTGRES_CONNECTION_STRING": "$DATABASE_URL"
      }
    },
    "github": {
      "type": "http",
      "url": "https://api.githubcopilot.com/mcp/",
      "auth": { "type": "oauth" }
    }
  }
}
```

### CLI commands

```bash
# Add a user-scoped MCP (available in all projects)
claude mcp add --scope user github --transport http https://api.githubcopilot.com/mcp/

# Add a project-scoped MCP (saved to .mcp.json)
claude mcp add --scope project postgres -- npx -y @modelcontextprotocol/server-postgres

# Add Context7 for live library docs
claude mcp add context7 -- npx -y @upstash/context7-mcp@latest

# List configured servers
claude mcp list

# Open MCP management panel inside Claude Code
# Type: /mcp
```

> **Security**: Always use `$ENV_VAR` syntax in `.mcp.json` — never hardcode API keys. Set the actual values in your shell environment or a `.env` file that is gitignored.

---

## settings.json — Permissions & Behaviour

`.claude/settings.json`

```json
{
  "model": "claude-sonnet-4-6",
  "permissions": {
    "allow": [
      "Bash(pnpm *)",
      "Bash(git status)",
      "Bash(git diff *)",
      "Bash(git log *)",
      "Read(*)",
      "Write(src/**)",
      "Write(tests/**)",
      "Write(docs/**)"
    ],
    "deny": [
      "Bash(rm -rf *)",
      "Bash(git push --force *)",
      "Bash(curl * | bash)",
      "Write(.env*)"
    ]
  },
  "attribution": {
    "commit": ""
  }
}
```

---

## Global Setup (~/.claude/)

Some configuration lives in your home directory and applies to all projects.

### `~/.claude/CLAUDE.md` — Global preferences

```markdown
# Global Preferences

## My Defaults
- I prefer TypeScript over JavaScript
- Always suggest error handling when writing async code
- Prefer named exports over default exports

## Commit Style
- Use conventional commits: feat:, fix:, chore:, docs:, refactor:
- Keep subject line under 72 chars
- No "Co-Authored-By" lines

## When Compacting
- Preserve the list of modified files
- Preserve current test status and any failing test names
```

---

## Planning Workflow

The highest-ROI workflow for production code:

```
Session A (Planning)
  → /spec "build [feature]"     # Claude interviews you
  → Review and approve SPEC.md
  → /plan                       # Claude phases the work
  → Review and approve PLAN.md

Session B (Implementation)       ← Fresh context, clean slate
  → "Execute Phase 1 from docs/PLAN.md"
  → Review, test, commit

Session C (Review)               ← Second Claude, no implementation bias
  → /review                     # Challenges every decision
  → Address feedback, final commit
```

### Context Management

```bash
# Compact when context gets long (keep the important stuff)
/compact focus on the current phase, modified files, and failing tests

# Clear entirely when switching tasks
/clear

# Check context usage
/context
```

---

## Session Best Practices

**Starting a session**
- Reference `docs/PLAN.md` at the start: *"Continue from Phase 2 in docs/PLAN.md"*
- Use `/clear` between unrelated tasks to avoid context bleed

**During a session**
- Correct Claude early if it goes off-track — the earlier, the cheaper
- Use `Esc` to stop mid-action without losing the session
- Use `/loop 5m check if tests are passing` to monitor CI in the background

**Ending a session**
- Ask Claude to update `docs/PLAN.md` with progress before closing
- Commit everything before ending — sessions are not saved automatically

**Verification**
- Don't accept "it should work" — always run: `pnpm typecheck && pnpm test`
- Challenge Claude: *"Prove to me this handles the edge case where X"*
- After a mediocre fix: *"Knowing everything you know now, implement the elegant solution"*

---

## Anti-Patterns to Avoid

| ❌ Don't | ✅ Do instead |
|---|---|
| Stuff every rule into CLAUDE.md | Keep CLAUDE.md < 200 lines; move extras to skills |
| `@-file` full docs in CLAUDE.md | Reference them: `"For X, see path/to/docs.md"` |
| `"Never use --foo-bar"` | `"Never use --foo-bar; prefer --baz instead"` |
| One giant session for a large feature | Plan → implement in phases → review in fresh sessions |
| Trust Claude's "looks good" | Run typecheck + tests yourself |
| Put secrets in `.mcp.json` | Use `$ENV_VAR` syntax |
| Write CLAUDE.md once and forget it | Treat it like code — review when things go wrong, prune regularly |

---

## Recommended MCP Stack

Start with 2-3 servers. Add more as you identify gaps.

| MCP Server | Purpose | Install |
|---|---|---|
| `@modelcontextprotocol/server-github` | PR reviews, issue management | `claude mcp add --transport http github https://api.githubcopilot.com/mcp/` |
| `@upstash/context7-mcp` | Live library documentation | `claude mcp add context7 -- npx -y @upstash/context7-mcp@latest` |
| `@modelcontextprotocol/server-postgres` | Direct DB access | `claude mcp add postgres -- npx -y @modelcontextprotocol/server-postgres` |
| `@modelcontextprotocol/server-sequential-thinking` | Complex multi-step reasoning | `claude mcp add sequential -- npx -y @modelcontextprotocol/server-sequential-thinking` |
| `perplexity-mcp` | Web research inside sessions | `claude mcp add perplexity -- npx -y perplexity-mcp` |
| `@playwright/mcp` | Browser automation & E2E testing | `claude mcp add playwright -- npx -y @playwright/mcp` |

---

## Resources

- [Official Claude Code Docs](https://docs.claude.com/en/docs/claude-code/overview)
- [Claude Code Best Practices](https://code.claude.com/docs/en/best-practices)
- [MCP Server Registry](https://modelcontextprotocol.io)
- [Writing a good CLAUDE.md](https://www.humanlayer.dev/blog/writing-a-good-claude-md)
- [Community best practices repo](https://github.com/shanraisshan/claude-code-best-practice)

---

*Last updated: April 2026 · Works with Claude Code (claude-sonnet-4-6 / claude-opus-4-6)*
