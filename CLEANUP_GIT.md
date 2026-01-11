# ?? Git Cleanup Instructions

## Problem
Git is tracking LARGE files that should be ignored:
- `.vs/` folder (Copilot snapshots - can be 100+ MB)
- `x64/` build outputs (.exe, .pdb files)

## Solution: Remove Tracked Files

Run these commands in PowerShell from your project root:

```powershell
# Navigate to project root
cd "E:\מבוא למדעי המחשב\Connect-4-v2"

# Remove .vs folder from Git (keeps local copy)
git rm -r --cached .vs/

# Remove x64 folder from Git (keeps local copy)
git rm -r --cached x64/

# Remove specific executables and debug files
git rm --cached x64/Debug/Connect-4-v2.exe
git rm --cached x64/Debug/Connect-4-v2.pdb

# Stage the .gitignore changes
git add Connect-4-v2/.gitignore

# Commit the cleanup
git commit -m "Remove build artifacts and IDE files from repository"

# Push to GitHub
git push origin main
```

## What This Does
- `git rm --cached` removes files from Git tracking but keeps them on your computer
- The updated `.gitignore` will prevent them from being added again
- Your repository will become MUCH smaller (probably 90% smaller!)

## Verify It Worked
```powershell
# Check what files are still tracked
git ls-files | Select-String ".vs"
git ls-files | Select-String "x64"
```

If these return nothing, you're clean! ?

## Optional: Clean Up GitHub History (Advanced)
If you want to remove these files from ALL previous commits (make repo even smaller):

**WARNING: This rewrites history - only do this if you're the only one working on the repo!**

```powershell
# Install BFG Repo-Cleaner (one time)
# Download from: https://rtyley.github.io/bfg-repo-cleaner/

# Or use git filter-branch (built-in but slower)
git filter-branch --tree-filter 'rm -rf .vs x64' --prune-empty HEAD
git push origin main --force
```

?? Only use `--force` if you're certain no one else has cloned your repo!
