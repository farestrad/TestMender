import google.generativeai as genai
import os

# Make sure GEMINI_API_KEY is set in your environment
# e.g. export GEMINI_API_KEY="your-key-here"
genai.configure(api_key=os.environ["GEMINI_API_KEY"])

response = genai.GenerativeModel("gemini-2.5-flash").generate_content(
    "Say hello world in Python"
)

print(response.text)
