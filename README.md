## 🧩 Setup Instructions

### 1. Create and activate a virtual environment
python3 -m venv .venv
source .venv/bin/activate

### 2. Install all dependencies
pip install -r requirements.txt

### 3. (Optional) Verify installation
python -m pip list

## 📦 Juliet Dataset (local-only)
This repo never stores the dataset; fetch it locally:
```bash
# If you have a local archive
scripts/setup_juliet.sh /path/to/Juliet-archive.tar.gz

# Or pass a URL
scripts/setup_juliet.sh https://example.com/Juliet-archive.tar.gz

# Or set env and run:
cp .env.example .env 