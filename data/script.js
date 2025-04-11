document.addEventListener("DOMContentLoaded", () => {
  const configForm = document.getElementById("config-form");
  const ssidInput = document.getElementById("ssid");
  const passwordInput = document.getElementById("password");
  const statusOutput = document.getElementById("status-output");
  const togglePasswordButton = document.getElementById("toggle-password");

  // Fetch saved WiFi credentials
  fetch("/config")
    .then(response => response.json())
    .then(data => {
      ssidInput.value = data.ssid || ""; // Populate SSID
      passwordInput.value = data.password || ""; // Populate password
    })
    .catch(err => {
      console.error("Error fetching configuration:", err.message);
    });

  // Fetch system status
  fetch("/status")
    .then(response => response.json())
    .then(data => {
      statusOutput.innerHTML = `
          <p><strong>Status:</strong> ${data.status}</p>
          <p><strong>IP Address:</strong> ${data.ip || "N/A"}</p>
        `;
    })
    .catch(err => {
      statusOutput.innerHTML = `<p>Error fetching status: ${err.message}</p>`;
    });

  // Handle form submission
  configForm.addEventListener("submit", (event) => {
    event.preventDefault();

    const formData = new FormData(configForm);
    const jsonData = JSON.stringify({
      ssid: formData.get("ssid"),
      password: formData.get("password")
    });

    fetch("/config", {
      method: "POST",
      headers: {
        "Content-Type": "application/json"
      },
      body: jsonData
    })
      .then(response => {
        if (response.ok) {
          alert("Configuration saved! Rebooting...");
        } else {
          alert("Failed to save configuration.");
        }
      })
      .catch(err => {
        alert("Error: " + err.message);
      });
  });

  // Toggle password visibility
  togglePasswordButton.addEventListener("click", () => {
    if (passwordInput.type === "password") {
      passwordInput.type = "text";
      togglePasswordButton.textContent = "Hide";
    } else {
      passwordInput.type = "password";
      togglePasswordButton.textContent = "Show";
    }
  });



  // Make items in IO Variable lists draggable
  document.querySelectorAll('#digital-inputs-list, #digital-outputs-list, #analog-inputs-list, #soft-io-list, #timers-list').forEach(el => {
    new Sortable(el, {
      group: { name: 'io-vars', pull: 'clone', put: false }, // Clone on drag, don't allow dropping back
      sort: false // Don't sort within the source list
    });
  });

  // Make Condition/Action builder areas drop targets
  new Sortable(document.getElementById('condition-builder-area'), {
    group: 'io-vars', // Accept drops from 'io-vars' group
    onAdd: function (evt) {
      // Handle item dropped here (e.g., populate form fields)
      console.log('IO Variable dropped into Conditions:', evt.item.textContent);
      evt.item.remove(); // Remove the clone after drop (or modify it)
    }
  });
  new Sortable(document.getElementById('action-builder-area'), {
    group: 'io-vars', // Accept drops from 'io-vars' group
    onAdd: function (evt) {
      console.log('IO Variable dropped into Actions:', evt.item.textContent);
      evt.item.remove();
    }
  });

  // Example for making Condition/Action Group lists sortable/droppable
  document.querySelectorAll('#condition-groups-list .sortable-list, #action-groups-list .sortable-list').forEach(el => {
    new Sortable(el, {
      group: 'conditions-or-actions', // Define groups for conditions/actions
      animation: 150
    });
  });


  // Make Rules list sortable
  new Sortable(document.getElementById('rules-list'), {
    group: 'rules',
    animation: 150,
    handle: '.list-group-item' // Allow dragging the whole item
  });

  // Add more Sortable initializations for dragging conditions/actions to groups, and groups to rules...
});