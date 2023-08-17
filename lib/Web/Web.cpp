#include "Web.h"
#include <Arduino.h>

const String Web_index = R"(
<!DOCTYPE html>
<html>

<head>
  <meta charset='utf-8'>
  <style>
    h1 {
      border-bottom: 1px solid #c0c0c0;
      margin-bottom: 10px;
      padding-bottom: 10px;
      white-space: nowrap;
    }

    table {
      border-collapse: collapse;
    }

    th {
      cursor: pointer;
    }

    td.detailsColumn {
      padding-inline-start: 2em;
      text-align: end;
      white-space: nowrap;
    }

    a.icon {
      padding-inline-start: 1.5em;
      text-decoration: none;
      user-select: auto;
    }

    a.icon:hover {
      text-decoration: underline;
    }

    a.file {
      background: url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAIAAACQkWg2AAAABnRSTlMAAAAAAABupgeRAAABEElEQVR42nRRx3HDMBC846AHZ7sP54BmWAyrsP588qnwlhqw/k4v5ZwWxM1hzmGRgV1cYqrRarXoH2w2m6qqiqKIR6cPtzc3xMSML2Te7XZZlnW7Pe/91/dX47WRBHuA9oyGmRknzGDjab1ePzw8bLfb6WRalmW4ip9FDVpYSWZgOp12Oh3nXJ7nxoJSGEciteP9y+fH52q1euv38WosqA6T2gGOT44vry7BEQtJkMAMMpa6JagAMcUfWYa4hkkzAc7fFlSjwqCoOUYAF5RjHZPVCFBOtSBGfgUDji3c3jpibeEMQhIMh8NwshqyRsBJgvF4jMs/YlVR5KhgNpuBLzk0OcUiR3CMhcPaOzsZiAAA/AjmaB3WZIkAAAAASUVORK5CYII=') left top no-repeat;
    }

    a.dir {
      background: url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAABt0lEQVR42oxStZoWQRCs2cXdHTLcHZ6EjAwnQWIkJyQlRt4Cd3d3d1n5d7q7ju1zv/q+mh6taQsk8fn29kPDRo87SDMQcNAUJgIQkBjdAoRKdXjm2mOH0AqS+PlkP8sfp0h93iu/PDji9s2FzSSJVg5ykZqWgfGRr9rAAAQiDFoB1OfyESZEB7iAI0lHwLREQBcQQKqo8p+gNUCguwCNAAUQAcFOb0NNGjT+BbUC2YsHZpWLhC6/m0chqIoM1LKbQIIBwlTQE1xAo9QDGDPYf6rkTpPc92gCUYVJAZjhyZltJ95f3zuvLYRGWWCUNkDL2333McBh4kaLlxg+aTmyL7c2xTjkN4Bt7oE3DBP/3SRz65R/bkmBRPGzcRNHYuzMjaj+fdnaFoJUEdTSXfaHbe7XNnMPyqryPcmfY+zURaAB7SHk9cXSH4fQ5rojgCAVIuqCNWgRhLYLhJB4k3iZfIPtnQiCpjAzeBIRXMA6emAqoEbQSoDdGxFUrxS1AYcpaNbBgyQBGJEOnYOeENKR/iAd1npusI4C75/c3539+nbUjOgZV5CkAU27df40lH+agUdIuA/EAgDmZnwZlhDc0wAAAABJRU5ErkJggg==') left top no-repeat;
    }

    a.up {
      background: url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAACM0lEQVR42myTA+w1RxRHz+zftmrbdlTbtq04qRGrCmvbDWp9tq3a7tPcub8mj9XZ3eHOGQdJAHw77/LbZuvnWy+c/CIAd+91CMf3bo+bgcBiBAGIZKXb19/zodsAkFT+3px+ssYfyHTQW5tr05dCOf3xN49KaVX9+2zy1dX4XMk+5JflN5MBPL30oVsvnvEyp+18Nt3ZAErQMSFOfelCFvw0HcUloDayljZkX+MmamTAMTe+d+ltZ+1wEaRAX/MAnkJdcujzZyErIiVSzCEvIiq4O83AG7LAkwsfIgAnbncag82jfPPdd9RQyhPkpNJvKJWQBKlYFmQA315n4YPNjwMAZYy0TgAweedLmLzTJSTLIxkWDaVCVfAbbiKjytgmm+EGpMBYW0WwwbZ7lL8anox/UxekaOW544HO0ANAshxuORT/RG5YSrjlwZ3lM955tlQqbtVMlWIhjwzkAVFB8Q9EAAA3AFJ+DR3DO/Pnd3NPi7H117rAzWjpEs8vfIqsGZpaweOfEAAFJKuM0v6kf2iC5pZ9+fmLSZfWBVaKfLLNOXj6lYY0V2lfyVCIsVzmcRV9Y0fx02eTaEwhl2PDrXcjFdYRAohQmS8QEFLCLKGYA0AeEakhCCFDXqxsE0AQACgAQp5w96o0lAXuNASeDKWIvADiHwigfBINpWKtAXJvCEKWgSJNbRvxf4SmrnKDpvZavePu1K/zu/due1X/6Nj90MBd/J2Cic7WjBp/jUdIuA8AUtd65M+PzXIAAAAASUVORK5CYII=') left top no-repeat;
    }

    html[dir=rtl] a {
      background-position-x: right;
    }

    #parentDirLinkBox {
      margin-bottom: 10px;
      padding-bottom: 10px;
    }
  </style>

  <title id='title'>Index of</title>

</head>

<body onload='readJSON()'>
  <h1 id='header'>Index of</h1>

  <form method='POST' action='/upload' enctype='multipart/form-data'><input type='file' name='file'
      id='fileInput'><input type='submit' value='Upload'></form>
  <progress id='progress' value='0' max='100' style='display: none;'></progress>
  <p id='fileSize'></p>
  <hr>
  <form action='/create' onsubmit='createFolder()'>
    <label for='folderName'>Folder name:</label>
    <input type='text' name='name' value='New'>
    <input type='submit' value='Create'>
  </form>
  <hr>
  <div id='parentDirLinkBox' style='display: none;'>
    <a id='parentDirLink' class='icon up' href='../'>
      <span id='parentDirText'>[parent directory]</span>
    </a>
  </div>
  <table id='directoryTable'>
    <thead>
      <tr>
        <th>Name</th>
        <th>Size</th>
        <th>Actions</th>
      </tr>
    </thead>
    <tbody>
      <!-- Table rows will be dynamically inserted here -->
    </tbody>
  </table>
  <hr>
  <script>
    function readJSON() {
      document.getElementById('progress').style.display = 'none';
      /* Get a reference to the table body */
      var tableBody = document.getElementById('directoryTable').getElementsByTagName('tbody')[0];

      /* Make a GET request to the '/json' endpoint */

      fetch(window.location.pathname + '?json')
        .then(response => response.json())
        .then(jsonData => {
          var uri = jsonData.path;

          document.getElementById('parentDirLinkBox').style.display = (uri == '/') ? 'hide' : 'block';
          document.getElementById('title').innerHTML = 'Index of ' + uri;
          document.getElementById('header').innerHTML = 'Index of ' + uri;

          /* Loop through the JSON data and generate table rows */
          jsonData.contents.forEach(function (item) {
            var row = tableBody.insertRow();
            var nameCell = row.insertCell(0);
            var sizeCell = row.insertCell(1);
            var actionsCell = row.insertCell(2);

            var nameLink = document.createElement('a');
            if (item.size != '') {
              nameLink.className = 'icon file';
              nameLink.href = uri + item.name;
              sizeCell.innerHTML = item.size + ' bytes'
            } else {
              nameLink.className = 'icon dir';
              nameLink.href = uri + item.name + '/';
              sizeCell.innerHTML = item.size;
            }

            nameLink.innerHTML = item.name;
            nameCell.appendChild(nameLink);

            var actionLink = document.createElement('a');
            actionLink.href = uri + item.name + '?delete';
            actionLink.innerHTML = 'Delete';
            actionsCell.appendChild(actionLink);
          });
        })
        .catch(error => {
          console.error('Error fetching JSON data:', error);
        });
    }
    document.getElementById('fileInput').addEventListener('change', updateFileSize);
    function updateFileSize() {
      var fileInput = document.getElementById('fileInput');
      var fileSize = 0;
      if (fileInput.files.length > 0) {
        fileSize = fileInput.files[0].size;
        document.getElementById('fileSize').innerHTML = 'Selected file size: ' + (fileSize / 1024).toFixed(2) + ' KB';
        document.getElementById('fileSize').style.display = 'block';
        document.getElementById('progress').style.display = 'block';
      } else {
        document.getElementById('fileSize').style.display = 'none';
        document.getElementById('progress').style.display = 'none';
      }

    }
    function deleteFile(filename) {
      if (confirm('Are you sure you want to delete ' + filename + '?')) {
        var xhr = new XMLHttpRequest();
        xhr.onreadystatechange = function () {
          if (xhr.readyState === 4 && xhr.status === 200) {
            window.location.reload();
          }
        };
        xhr.open('GET', filename + '?delete=', true);
        xhr.send();
      }
    }
    function uploadFile() {
      var fileInput = document.getElementById('fileInput');
      var file = fileInput.files[0];
      var xhr = new XMLHttpRequest();
      xhr.onreadystatechange = function () {
        if (xhr.readyState === 4 && xhr.status === 200) {
          document.getElementById('status').innerHTML = 'File uploaded successfully.';
        }
      };
      xhr.open('POST', '/upload', true);
      var formData = new FormData();
      formData.append('file', file, file.name);
      xhr.send(formData);
    }
    function updateProgress() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          /* content += '                        document.getElementById('millisValue').innerText = this.responseText; */
          document.getElementById('progress').value = this.responseText;
          setTimeout(function () { updateProgress(); }, 100);
        }
      };
      xhttp.open('GET', '/index.php?progress', true);
      xhttp.send();
    }
    function createFolder() {
      var uri = window.location.pathname;
      var xhr = new XMLHttpRequest();
      xhr.open('GET', '/index.php?create=' + uri + '/' + filename, true);
      xhr.send();
    }
  </script>
</body>

</html>
)";