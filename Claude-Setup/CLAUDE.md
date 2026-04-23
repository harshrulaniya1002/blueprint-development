# Claude Code Setup Template

## Overview
This project scaffold contains a production-grade Claude Code setup for local projects.
It includes shared session context, project commands, subagents, skills, rules, hooks, MCP configuration, and project-level setup guidance.

## Stack
- Claude Code / Claude AI
- Local project integration via `.claude/` and `CLAUDE.md`

## Project Structure
- `CLAUDE.md` — team-shared context and session anchor
- `CLAUDE.local.md` — personal preferences, gitignored
- `.mcp.json` — project MCP server config
- `.claude/settings.json` — Claude permissions and hook config
- `.claude/commands/` — slash commands for plans, review, spec
- `.claude/agents/` — subagent definitions
- `.claude/skills/` — optional domain knowledge packages
- `.claude/rules/` — automatic behavioural rules
- `.claude/hooks/` — deterministic hook scripts
- `docs/SPEC.md` — architecture and feature decisions
- `docs/PLAN.md` — working plan and progress

## Essential Commands
```bash
# Use this scaffold inside a local project root
ls .claude
cat CLAUDE.md
cat .claude/settings.json
```

## How to Use This Template
1. Copy the files into your local project root.
2. Update `CLAUDE.md` with project-specific stack, commands, and conventions.
3. Add `CLAUDE.local.md` to your project `.gitignore`.
4. Configure `.mcp.json` with any project-specific external tools.
5. Customize `.claude/settings.json` permissions and hooks for your repo.
6. Use `.claude/commands` for reusable slash workflows such as `/plan`, `/review`, and `/spec`.

## Local Project Integration
- Keep this scaffold in a reference folder or template repo.
- Copy only the files you need into the target project root.
- Leave `CLAUDE.md` and `.mcp.json` at root level.
- Keep `.claude/` as a hidden configuration folder.
- Use `docs/` for shared design decisions and working plans.
