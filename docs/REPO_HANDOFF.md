# Publishing this repo to GitHub

The ZIP contains a complete local Git repository including the `.git` directory and reconstructed incremental history. Its 35 supplied milestone commits are preserved in the published repository at https://github.com/DileepaNayanajith/mochi-ai-chatbot-journey. The steps below describe publishing a separate copy.

```bash
unzip mochi-ai-chatbot-journey-git-history.zip
cd mochi-ai-chatbot-journey
git log --oneline --reverse
# Create an empty repository through GitHub UI, then:
git remote add origin https://github.com/YOUR_USERNAME/mochi-ai-chatbot-journey.git
git push -u origin main
```

Before pushing, set the author email to your own in subsequent commits: `git config user.email 'your-github-noreply@example.com'` and replace `your-github-noreply@example.com` with your real GitHub no-reply address. The packaging-time reconstructed commits use placeholder `local-development@localhost`, **not your personal email**. If GitHub attribution of the reconstructed commits matters, use `git filter-repo` or interactive history editing carefully before publishing.

Do not add model weights, the user's private Wi-Fi credentials or personal voice recordings. `git status --short` should be empty after extraction. Verified/claimed milestones are separated from future work in the README and roadmap.
