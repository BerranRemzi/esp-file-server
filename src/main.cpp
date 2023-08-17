#include <Arduino.h>
#include <config.h>
#include <Web.h>
#include <ESP8266WiFi.h>
#include <FS.h>
#include <SD.h>
#include <LittleFS.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

/* Macros */
#ifdef LITTLE_FS
#define FILE_SYSTEM LittleFS
#else
#define FILE_SYSTEM SD
#endif

/* typedef's and structs */
typedef struct
{
  void (*p_Request)(AsyncWebServerRequest *);
  const String param;
} Handler_Function_t;

/* Consts */
const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;

/* Local variables and classes */
AsyncWebServer server(80);
long progress = 0;

/* Local function prototypes */
String AddSlashIfNeeded(String inputString);

void Handler_Upload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);

void Handler_Main(AsyncWebServerRequest *request);
void Handler_Index(AsyncWebServerRequest *request);
void Handler_UploadProgress(AsyncWebServerRequest *request);
void Handler_Delete(AsyncWebServerRequest *request);
void Handler_Create(AsyncWebServerRequest *request);
void Handler_Download(AsyncWebServerRequest *request);
void Handler_Json(AsyncWebServerRequest *request);

/**
 * Adds a trailing slash to the input string if one is not already present.
 *
 * @param inputString The input string to which a trailing slash may be added.
 * @return The modified input string with a trailing slash if needed.
 */
String AddSlashIfNeeded(String inputString)
{
  if (inputString.length() > 0)
  {
    // Check if the last character of the string is '/'
    if (inputString.charAt(inputString.length() - 1) != '/')
    {
      inputString += '/';
    }
  }
  else
  {
    inputString = "/";
  }
  return inputString;
}

/* Local function bodies */
/**
 * Main request handler for the AsyncWebServer.
 *
 * @param request A pointer to the incoming AsyncWebServerRequest.
 */
void Handler_Main(AsyncWebServerRequest *request)
{
#define HANDLER_FUNCTION_COUNT 5u
  const Handler_Function_t handler[HANDLER_FUNCTION_COUNT] = {
      {Handler_UploadProgress, "progress"},
      {Handler_Delete, "delete"},
      {Handler_Download, "download"},
      {Handler_Json, "json"},
      {Handler_Index, "dir"}};

  bool handlerRequested = false;

  for (uint8_t i = 0; i < HANDLER_FUNCTION_COUNT; i++)
  {
    if (request->hasParam(handler[i].param))
    {
      handler[i].p_Request(request);
      handlerRequested = true;
      break;
    }
  }
  if (false == handlerRequested)
  {
    handler[HANDLER_FUNCTION_COUNT - 1u].p_Request(request);
  }
}

/**
 * Handler function for serving the index HTML page.
 *
 * @param request A pointer to the incoming AsyncWebServerRequest.
 */
void Handler_Index(AsyncWebServerRequest *request)
{
  request->send(200, "text/html", Web_index);
}

/**
 * Handler function for uploading files.
 *
 * @param request   A pointer to the incoming AsyncWebServerRequest.
 * @param filename  The name of the file being uploaded.
 * @param index     The index indicating the position in the upload sequence.
 * @param data      A pointer to the uploaded data buffer.
 * @param len       The length of the data in the buffer.
 * @param final     Indicates if this is the final part of the upload.
 */
void Handler_Upload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
  static File file;
  static size_t fileSize = 0;
  const char *uploadStatus = "Idle";

  if (!index)
  {
    file = FILE_SYSTEM.open("/" + filename, "w");
    fileSize = request->contentLength();
  }

  progress = (((index + len) * 100) / fileSize);

  if (file)
  {
    file.write(data, len);

    if (final)
    {
      file.close();
      uploadStatus = "File uploaded successfully.";
      progress = 100;
      // Send an empty response to indicate completion and redirect to the home page
      // request->send(200, "text/plain", "/");
      request->redirect("/");
    }
  }
  else
  {
    progress = 0;
    uploadStatus = "Error uploading file.";
    // request->send(500, "text/plain", uploadStatus);
    request->redirect("/");
  }
}

/**
 * Handler function for reading upload progress percentage.
 *
 * @param request A pointer to the incoming AsyncWebServerRequest.
 */
void Handler_UploadProgress(AsyncWebServerRequest *request)
{
  request->send(200, "text/plain", String(progress));
}

/**
 * Handler function for deleting file or folder.
 *
 * @param request A pointer to the incoming AsyncWebServerRequest.
 */
void Handler_Delete(AsyncWebServerRequest *request)
{
  String filename = request->url();
  if (FILE_SYSTEM.remove(filename))
  {
    // request->send(200, "text/plain", "File deleted successfully.");
  }
  else
  {
    // request->send(500, "text/plain", "Error deleting file.");
  }
  request->redirect("/");
}

/**
 * Handler function for creatin folder.
 *
 * @param request A pointer to the incoming AsyncWebServerRequest.
 */
void Handler_Create(AsyncWebServerRequest *request)
{
  String folderPath = request->url();
  if (FILE_SYSTEM.mkdir(folderPath))
  {
    request->send(200, "text/plain", "Folder created: " + folderPath);
  }
  else
  {
    request->send(500, "text/plain", "Failed to create folder");
  }
  request->redirect("/");
}
const size_t JSON_BUFFER_SIZE = 16 * 1024; // Adjust as needed
StaticJsonDocument<JSON_BUFFER_SIZE> jsonDocument;

/*
 *
 */
void Handler_Json(AsyncWebServerRequest *request)
{

  jsonDocument.clear();

  String path = AddSlashIfNeeded(request->url()); // Get the URL from the request
  Serial.println(path);
  jsonDocument["path"] = path;
  JsonArray filesArray = jsonDocument.createNestedArray("contents");

#if LITTLE_FS
  Dir dir = FILE_SYSTEM.openDir(path);
  while (dir.next())
  {
    File entry = dir.openFile("r");
#else
  File dir = FILE_SYSTEM.open(path);
  File entry = dir.openNextFile();
  while (entry)
  {
#endif
    size_t fileSize = entry.size();

    JsonObject fileInfo = filesArray.createNestedObject();
    fileInfo["name"] = String(entry.name());
    fileInfo["size"] = entry.isDirectory() ? "" : String(fileSize);

    entry.close();
#if !LITTLE_FS
    entry = dir.openNextFile();
    if (!entry)
    {
      break;
    }
#endif
  }
  // Serialize the JSON object to a string
  String jsonString;
  serializeJson(jsonDocument, jsonString);

  // Print the JSON string
  request->send(200, "text/plain", jsonString);
}

/**
 * Handler function for downloading files.
 *
 * @param request A pointer to the incoming AsyncWebServerRequest.
 */
void Handler_Download(AsyncWebServerRequest *request)
{
  if (request->hasParam("download"))
  {
    String filename = request->getParam("download")->value();

    File file = FILE_SYSTEM.open("/" + filename, "r");

    request->send(file, filename, String(), true); // Display file
  }
}

void setup()
{
  Serial.begin(9600);
#if LITTLE_FS
  Serial.print("\r\nWaiting for SPIFF card to initialise...");
  if (!FILE_SYSTEM.begin())
  {
    Serial.println("Initialising failed!");
    delay(500);
    ESP.restart();
    return;
  }
#else
  Serial.print("\r\nWaiting for SD card to initialise...");
  pinMode(5, OUTPUT);
  if (!FILE_SYSTEM.begin(5, 40000000))
  {
    Serial.println("Initialising failed!");
    delay(500);
    ESP.restart();
    return;
  }
#endif /* LITTLE_FS */

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi.");
  Serial.println((WiFi.localIP().toString()));

  server.on(
      "/upload", HTTP_POST, [](AsyncWebServerRequest *request)
      {
        request->send(200, "text/html", ""); // Respond immediately to the file upload request
      },
      Handler_Upload);

  // server.on("/delete", HTTP_GET, Handler_DeleteFile);

  // Route for creating a folder
  server.on("/create", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
              String folderName = request->getParam("name")->value();
    
              if (FILE_SYSTEM.mkdir(folderName)) {
                request->send(200, "text/plain", "Folder created: " + folderName);
              } else {
                request->send(500, "text/plain", "Failed to create folder");
              }
              request->redirect("/"); });

  // server.on("/progress", Handler_UploadProgress);
  // server.on("/json", HTTP_GET, Handler_Json);

  //server.on("/index.php", HTTP_GET, Handler_Download);
  server.on("/", HTTP_GET, Handler_Index);

  // Default handler for non-existent routes

  //server.onNotFound(Handler_Main);

  server.begin();
}

void loop()
{
}