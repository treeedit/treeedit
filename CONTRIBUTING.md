# Contributing Guide

## Setup

```
brew install git git-lfs python3
brew install cmake imagemagick qt svgo
brew install --cask db-browser-for-sqlite qt-creator

python3 -m venv ~/treeedit
source ~/treeedit/bin/activate
pip3 install -r requirements.txt
```

## Run

```
cd docs
mkdocs serve
```

Go to [http://127.0.0.1:8000](http://127.0.0.1:8000) to see the docs.
