document.addEventListener("DOMContentLoaded", () => {
    const configForm = document.getElementById("config-form");
    const statusOutput = document.getElementById("status-output");
  
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
  });