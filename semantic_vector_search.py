"""
This script is an offline intelligent semantic search tool for medical question and answer databases, focused on topics such as antibiotics and antiviral drugs. The core principle is to use a deep learning natural language model—specifically, a multilingual SentenceTransformer model—to convert both users' natural language queries and the database's question entries into high-dimensional semantic vectors that capture meaning and context across many languages.

After all database question entries have been converted into sentence embeddings, the script uses the FAISS library to build a vector index. When the user enters a query, the program encodes the query into a sentence vector as well, then uses cosine similarity to search the index efficiently. Cosine similarity measures the angle between the semantic vectors of the query and each database entry, so it reflects their semantic closeness rather than matching exact words or word order. This way, the tool can find the most relevant answers even if the user's question is phrased differently or in a different language.

The model for encoding text is downloaded once and then stored locally, allowing fully offline use after the initial download. The combination of SentenceTransformer for semantic understanding and FAISS for fast similarity search gives this program high speed and accuracy, supporting both short and long queries, and allowing the answer text to be rich in format and content.
"""

from sentence_transformers import SentenceTransformer
import numpy as np
import faiss
import os

MODEL_DIR = 'models/paraphrase-multilingual-MiniLM-L12-v2'

def prepare_model(model_dir=MODEL_DIR):
    """
    Loads a BERT-based SentenceTransformer model from the local directory if available.
    If not, downloads the model and saves it for future offline use.
    Returns a SentenceTransformer model object.
    """
    model_exists = os.path.isdir(model_dir) and os.path.isfile(os.path.join(model_dir, 'config.json'))
    if model_exists:
        print("Loading local semantic model from", model_dir)
        model = SentenceTransformer(model_dir)
    else:
        print("Local model not found. Downloading from HuggingFace...")
        model = SentenceTransformer('paraphrase-multilingual-MiniLM-L12-v2')
        model.save(model_dir)
        print("Model downloaded and saved locally at", model_dir)
    return model

model = prepare_model()

# The question-answer knowledge base, with typical questions as keys and their answers as values
data = {
    "What are antibiotics?": """
Antibiotics are medicines used to prevent and treat bacterial infections.
They do not work against viral infections such as flu or colds.
Misuse of antibiotics can lead to antibiotic resistance, making infections harder to treat in the future.
""",
    "When should I take antibiotics?": """
You should only take antibiotics when prescribed by a qualified healthcare professional, usually for confirmed or highly suspected bacterial infections.
Antibiotics are not effective against viruses.
Improper use increases the risk of resistance and side effects.
""",
    "Can antibiotics treat viral infections?": """
No, antibiotics do not work on viruses.
Common viral illnesses such as the common cold, flu, or COVID-19 cannot be cured with antibiotics.
Antibiotics are only effective against bacterial infections.
""",
    "What are antiviral drugs?": """
Antiviral drugs are medications designed to treat viral infections by inhibiting the development and replication of viruses.
They are not the same as antibiotics.
Some antivirals are specific to certain viruses, such as HIV, influenza, or herpesviruses.
""",
    "How do antiviral drugs work?": """
Antiviral drugs often inhibit virus replication, viral entry into cells, or assembly of new viruses.
They may target different stages of the viral life cycle, depending on the specific medication.
Unlike vaccines, antivirals help treat or lessen the severity of a viral infection after infection occurs.
""",
    "Can I stop antibiotics when I feel better?": """
No, you should always complete the full course of antibiotics prescribed to you, even if you feel better before the medication runs out.
Stopping early can allow some bacteria to survive, increasing the chance of reinfection and antibiotic resistance.
""",
    "What is antibiotic resistance?": """
Antibiotic resistance occurs when bacteria adapt and become less susceptible or unresponsive to the effects of antibiotics.
Causes include overuse or misuse of antibiotics, such as treating viral infections or not completing courses.
It is a serious global health threat, making infections harder and more expensive to treat.
""",
    "Are antiviral drugs effective for all viruses?": """
No, most antiviral drugs are specific to certain viruses.
There is not a universal antiviral drug effective for all viral infections.
Treatment depends on the virus, health condition, and available medications.
"""
}

# Prepare the keys and encode them as sentence embeddings
key_list = list(data.keys())
key_embeddings = model.encode(key_list, convert_to_numpy=True).astype('float32')
key_embeddings /= np.linalg.norm(key_embeddings, axis=1, keepdims=True)

# Build a FAISS index for efficient similarity search using cosine similarity
faiss_index = faiss.IndexFlatIP(key_embeddings.shape[1])
faiss_index.add(key_embeddings)

print("Enter your antibiotic or antiviral drug question. Type q or quit to exit.")

while True:
    user_query = input(">>> ").strip()
    if user_query.lower() in ['q', 'quit', 'exit']:
        print("Exiting search.")
        break
    query_embedding = model.encode([user_query], convert_to_numpy=True).astype('float32')
    query_embedding /= np.linalg.norm(query_embedding, axis=1, keepdims=True)
    topN = 3
    D, I = faiss_index.search(query_embedding, topN)
    print("Most relevant answers:")
    for idx, score in zip(I[0], D[0]):
        if score < 0.3:
            continue
        question = key_list[idx]
        print(question)
        print(data[question].strip())
        print()
    print("---------------------------------------------")
