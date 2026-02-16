# Contributing Guidelines

Welcome to the project.

This repository follows a structured, phase-based workflow. All contributions must comply with the rules below.

---

## Branching Model

We use the following branches:

- `main` — Stable production branch (protected)
- `dev` — Integration branch
- `redundancy` — Last stable completed-task snapshot
- `feat/*` — Feature work
- `fix/*` — Bug fixes
- `infra/*` — Infrastructure updates
- `ui/*` — UI-related tasks
- `art/*` — Art-related tasks

Direct pushes to `main` are prohibited.

---

## Branch Naming Convention

Branch names must follow:

type/task-description

makefile
Copy code

Examples:

feat/inventory-base
infra/bot-command-loader
art/blockout-kit

yaml
Copy code

Invalid branch names will fail validation.

---

## Pull Request Rules

All changes must:

- Be submitted via Pull Request
- Target `dev` first (unless instructed otherwise)
- Receive at least 1 approval
- Pass branch name validation
- Resolve all review conversations

Squash merging is preferred.

---

## Unreal Engine Asset Rules

Before committing:

- Ensure Git LFS is installed
- Never commit:
  - `Binaries/`
  - `DerivedDataCache/`
  - `Intermediate/`
  - `Saved/`

Only commit:

- `.uproject`
- `Config/`
- `Content/`
- Source code (if applicable)

---

## Task Workflow

1. Claim task via Discord.
2. Create a branch following naming convention.
3. Complete task.
4. Open PR.
5. Wait for review.
6. Merge into `dev`.
7. When validated, `dev` merges into `main`.

A task is not complete until merged.

---

## Code & Asset Discipline

- Keep commits focused.
- Do not bundle unrelated changes.
- Keep test files in `/Content/Dev/`.
- Follow naming conventions for assets.

---

## Questions & Blockers

If blocked for more than 24 hours:
- Post in the task thread.
- Do not proceed in isolation.

We are building the machine before building the product.
Discipline first. Features second.
