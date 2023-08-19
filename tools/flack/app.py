from flask import Flask, request, jsonify, render_template, send_from_directory
import os

app = Flask(__name__)

@app.route("/", methods=["POST"])
def upload():
    file = request.files["file"]
    if file:
        filename = file.filename
        sd_dir = os.path.join(app.root_path, "sd")
        file.save(os.path.join(sd_dir, filename))
        return "File uploaded successfully!"
    return "No file uploaded."


@app.route("/")
@app.route("/<path:path>", methods=["GET"])
def serve(path="/"):
    if path != "/":
        path = "/" + path
    sd_dir = os.path.join(app.root_path, "sd")
    path_full = sd_dir + "/" + path
    request_args = request.args
    if "json" in request_args:
        # Return JSON data of directory contents
        contents = []
        for item in os.listdir(path_full):
            item_path = os.path.join(path_full, item)
            item_info = {
                "name": item,
                "size": os.path.getsize(item_path) if os.path.isfile(item_path) else "",
                "date": os.path.getmtime(item_path),
            }
            contents.append(item_info)
        return jsonify({"path": path, "contents": contents})
    elif "delete" in request_args:
        path = path_full
        # Delete file or directory
        if os.path.exists(path):
            try:
                if os.path.isfile(path):
                    os.remove(path)
                elif os.path.isdir(path):
                    os.rmdir(path)
                return f"Deleted: {path}"
            except OSError as e:
                return f"Error occured"
        return f"Path not found: {path}"
    elif "create" in request_args:
        path = path_full
        # Create new directory
        folder_name = os.path.basename(path)
        new_folder_path = os.path.join(os.path.dirname(path), folder_name)
        if not os.path.exists(new_folder_path):
            os.makedirs(new_folder_path)
            return f"Created folder: {new_folder_path}"
        return f"Folder already exists: {new_folder_path}"
    else:
        return render_template(
            "index.html"
        )  # Place your HTML file in a "templates" folder


if __name__ == "__main__":
    app.run(debug=True)
