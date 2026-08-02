from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route('/log', methods=['POST'])
def log_data():
    data = request.json
    light_level = data.get('light_level')
    status = data.get('status')

    print(f"Light Level: {light_level} | Status: {status}")
    # In a real deployment, this would be saved to a database
    # e.g. SQLite, PostgreSQL, or a simple CSV log file

    return jsonify({"message": "Logged successfully"}), 200


@app.route('/', methods=['GET'])
def home():
    return "Solar Street Light Monitoring Server is running."


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
