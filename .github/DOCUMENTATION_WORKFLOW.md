# Documentation Workflow Guide

This document explains how the documentation generation workflow works and how to use it.

## Overview

The `generate-docs.yml` workflow automatically generates API documentation using Doxygen and optionally publishes it to the GitHub Wiki.

## Triggers

The workflow can be triggered in two ways:

1. **Automatically**: Called by other workflows (using `workflow_call`)
2. **Manually**: Via GitHub Actions UI (using `workflow_dispatch`)

## Manual Triggering

To manually generate and publish documentation:

1. Go to the **Actions** tab in the repository
2. Select **"Generate Documentation"** from the workflows list on the left
3. Click the **"Run workflow"** button (top right)
4. Select the branch you want to generate documentation from
5. Click **"Run workflow"** to start the process

## Workflow Steps

1. **Checkout code**: Checks out the repository
2. **Install Doxygen and Graphviz**: Installs required documentation tools
3. **Generate documentation**: Runs Doxygen to create HTML documentation in `docs/html/`
4. **Checkout Wiki** (optional): Attempts to checkout the GitHub Wiki repository
5. **Copy documentation to Wiki** (conditional): Copies generated docs to Wiki if it's available
6. **Commit and push to Wiki** (conditional): Publishes updated documentation to Wiki
7. **Wiki not available** (conditional): Shows helpful message if Wiki is not enabled

## Wiki Setup

The workflow will gracefully handle the case where the Wiki is not yet enabled. If you want to enable Wiki publishing:

1. Go to repository **Settings**
2. Scroll down to the **Features** section
3. Enable **Wikis**
4. Create at least one page in the Wiki (this initializes the Wiki repository)

Once the Wiki is enabled and initialized, the workflow will automatically publish documentation to it.

## Without Wiki

Even if the Wiki is not available, the workflow will still:
- Successfully generate the documentation
- Complete without errors
- Store the generated HTML in the `docs/html/` directory (gitignored)

The documentation can be viewed locally by opening `docs/html/index.html` in a web browser after running the workflow or generating docs locally.

## Local Documentation Generation

To generate documentation locally without running the workflow:

```bash
# Install Doxygen and Graphviz (if not already installed)
sudo apt-get install -y doxygen graphviz  # Ubuntu/Debian
brew install doxygen graphviz              # macOS

# Generate documentation
doxygen Doxyfile

# View documentation
open docs/html/index.html  # macOS
xdg-open docs/html/index.html  # Linux
```

## Troubleshooting

### Workflow fails with "repository not found"

This error occurs when the Wiki is not enabled. The workflow has been updated to handle this gracefully:
- The error is now caught with `continue-on-error: true`
- Documentation is still generated successfully
- A helpful notice is displayed with instructions to enable the Wiki

### Documentation not updating in Wiki

If documentation generation succeeds but Wiki doesn't update:
1. Check that the Wiki is enabled in repository settings
2. Verify that at least one Wiki page exists (to initialize the repository)
3. Check workflow logs for any permission errors
4. Ensure the `GITHUB_TOKEN` has `contents: write` permission (already configured)

## Related Documentation

- [Release Workflow Setup](.github/RELEASE_WORKFLOW_SETUP.md)
- [GitHub Actions Documentation](https://docs.github.com/en/actions)
- [Doxygen Manual](https://www.doxygen.nl/manual/)
