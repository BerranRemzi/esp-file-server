#include "Web.h"
#include <Arduino.h>

const char Web_index[] PROGMEM = R"(
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

<body onload='loadDirContents()'>
  <h1 id='header'>Index of</h1>

  <input type='file' id='fileInput'>
  <button onclick='uploadFile()'>Upload</button>
  <progress id='progress' value='0' max='100' style='display: none;'></progress>
  <p id='fileSize'></p>
  <hr>
  <label for='folderName'>Folder name:</label>
  <input type='text' id='folderNameInput' value='New'>
  <input type='button' value='Create' onclick='performOperation(`create`)'>
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
        <th>Modified</th>
        <th>Actions</th>
      </tr>
    </thead>
    <tbody>
      <!-- Table rows will be dynamically inserted here -->
    </tbody>
  </table>
  <hr>
  <label id='response'></label>
  <script>
    const parentDirLinkBox = document.getElementById('parentDirLinkBox');
    const title = document.getElementById('title');
    const header = document.getElementById('header');
    const tableBody = document.querySelector('#directoryTable tbody');
    const fileInput = document.getElementById('fileInput');
    const fileSizeElement = document.getElementById('fileSize');
    const progressElement = document.getElementById('progress');
    const responseElement = document.getElementById('response');
    const folderNameInput = document.getElementById('folderNameInput');

    async function loadDirContents() {
      try {
        const response = await fetch(`${window.location.pathname}?json`);
        const jsonData = await response.json();

        const uri = jsonData.path;
        parentDirLinkBox.style.display = (uri === '/') ? 'none' : 'block';
        title.textContent = header.textContent = `Index of ${uri}`;

        tableBody.innerHTML = ''; // Clear table body

        jsonData.contents.forEach(item => {
          const row = tableBody.insertRow();
          const [nameCell, sizeCell, dateCell, actionsCell] = Array.from({ length: 4 }, () => row.insertCell());

          const nameLink = document.createElement('a');
          const actionButton = document.createElement('button');

          if (item.size !== '') {
            nameLink.classList.add('icon', 'file');
            nameLink.href = `${uri}${item.name}`;
            sizeCell.textContent = `${item.size} bytes`;
          } else {
            nameLink.classList.add('icon', 'dir');
            nameLink.href = `${uri}${item.name}/`;
          }

          nameLink.textContent = item.name;
          nameCell.appendChild(nameLink);

          actionButton.textContent = 'Delete';
          actionButton.onclick = () => performOperation('delete', item.name);

          actionsCell.appendChild(actionButton);
          dateCell.textContent = new Date(item.date * 1000).toLocaleString();
        });
      } catch (error) {
        console.error('Error fetching JSON data:', error);
      }
    }

    document.getElementById('fileInput').addEventListener('change', updateFileSize);

    function updateFileSize() {
      const fileSize = fileInput.files.length > 0 ? fileInput.files[0].size : 0;
      fileSizeElement.textContent = fileSize > 0 ? `Selected file size: ${fileSize} bytes` : '';
    }

    function updateUploadProgress(percentage) {
      progressElement.value = percentage;
      progressElement.style.display = (percentage > 0 && percentage < 100) ? 'block' : 'none';
    }

    async function uploadFile() {
      const file = fileInput.files[0];

      if (!file) {
        alert('Please select a file to upload.');
        return;
      }

      const formData = new FormData();
      formData.append('file', file);

      try {
        const xhr = new XMLHttpRequest();
        xhr.open('POST', '/', true);

        xhr.upload.addEventListener('progress', event => {
          if (event.lengthComputable) {
            const percentCompleted = Math.round((event.loaded / event.total) * 100);
            updateUploadProgress(percentCompleted);
          }
        });

        xhr.onload = () => {
          if (xhr.status === 200) {
            updateUploadProgress(100);
            fileInput.value = '';
            updateFileSize();
          } else {
            console.error('Error:', xhr.statusText);
          }
          responseElement.textContent = xhr.responseText;
          loadDirContents();
        };

        xhr.onerror = () => {
          console.error('Request failed');
        };

        xhr.send(formData);
      } catch (error) {
        console.error('Request failed:', error);
      }
    }

    async function performOperation(operation, itemName = folderNameInput.value) {
      const uri = window.location.pathname;
      const newUri = `${uri}${itemName}${operation === 'create' ? '/' : ''}`;
      try {
        const response = await fetch(`${newUri}?${operation}`);
        const responseText = await response.text();
        responseElement.textContent = responseText;
      } catch (error) {
        responseElement.textContent = `An error occurred: ${error}`;
      }
      loadDirContents();
    }
  </script>
</body>

</html>
)";