---
title: Features
---

# Features

## Markdown

Tree Edit uses the [`QTextDocument::setMarkdown`](https://doc.qt.io/qt-6/qtextdocument.html#setMarkdown) function to set text as Markdown using the CommonMark dialect.

For a complete list of supported features of the CommonMark Markdown dialect, please see the [official CommonMark specification](https://commonmark.org/help).

### Markdown examples

| Markdown | Example |
|---|---|
| `*italic*` | *italic* |
| `**bold**` | **bold** |
| `[link](https://bing.com)` | [link](https://bing.com) |
| `` `code` `` | `code` |

## Typography

### Inter

[Inter font](https://fonts.google.com/specimen/Inter) is used for [`DisplayRole`](https://doc.qt.io/qt-6/qt.html#ItemDataRole-enum), which supports ligatures, thus displaying information nicely.
