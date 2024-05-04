# Git

Did you know you can commit the Sqlite file that Tree Edit uses to persist the notes to Git?
The problem is that the Sqlite files are binary, but don't fret!
You can get nice diffs by setting up Git to show you the difference in SQL commands between what's already committed and what's about to be committed in a pull request.

First, run the following commands in your git repository.
This tells Git to use the `sqlite3` command to output SQL.

```
git config diff.sqlite3.binary true
git config diff.sqlite3.textconv "echo .dump | sqlite3"
```

Second, add a `.gitattributes` file by following the instructions from
https://github.com/treeedit/treeedit/blob/main/.gitattributes

![VS Code Sqlite Git Diff](/images/vscode-git-diff.png)
