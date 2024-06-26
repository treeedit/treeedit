---
title: Tracking Sqlite database file changes with Git
---

# Tracking Sqlite database file changes with Git

Tree Edit stores the tree nodes in a Sqlite database file.

Did you know you can commit the Sqlite file that Tree Edit uses to persist the notes to Git?
The problem is that the Sqlite files are binary, but don't fret!
You can get nice diffs by setting up Git to show you the difference in SQL commands between what's already committed and what's about to be committed in a pull request.

First, run the following commands in your git repository.
This tells Git to use the `sqlite3` command to output SQL.

```
git config diff.sqlite3.binary true
git config diff.sqlite3.textconv "echo .dump | sqlite3"
```

Second, add a `.gitattributes` file by following the instructions from [Tree Edit `.gitattributes`](https://github.com/treeedit/treeedit/blob/main/.gitattributes).

```
*.sqlite diff=sqlite3
*.db     filter=lfs diff=sqlite3 merge=lfs -text
```
???+ note "Git large file storage (LFS)"

    The `-text` argument tells the large file storage extension that this is **not** a text file, thus should not normalize line endings. For more information, please see the [Git end-of-line conversion docs](https://git-scm.com/docs/gitattributes#_end_of_line_conversion).

The `.sqlite` files are tracked by Git and `.db` files are tracked by Git large file storage (LFS) as an example.
LFS files can't be reverted to previous versions, so you may want to stick with the standard approach of committing Sqlite binaries to source control as if they're text files, albeit that's a big no no in the industry.

Once you finish the setup, here's what you'll get in VS Code when committing Sqlite files to Git.
It displays the SQL commands that have to be run to update the database from one version to another.

![VS Code Sqlite Git Diff](/images/vscode-git-diff.png)
