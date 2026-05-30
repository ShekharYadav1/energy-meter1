#ifndef HTMLFILE_H
#define HTMLFILE_H

// ========== Combined WiFi + Bot ID Configuration Page ==========
const char configPageHTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name='viewport' content='width=device-width, initial-scale=1, user-scalable=yes'>
    <title>ESP32 Energy Meter - Setup</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            padding: 20px;
            display: flex;
            justify-content: center;
            align-items: center;
        }
        
        .container {
            max-width: 500px;
            width: 100%;
            background: white;
            border-radius: 20px;
            box-shadow: 0 20px 60px rgba(0,0,0,0.3);
            overflow: hidden;
            animation: slideIn 0.5s ease-out;
        }
        
        @keyframes slideIn {
            from {
                opacity: 0;
                transform: translateY(-30px);
            }
            to {
                opacity: 1;
                transform: translateY(0);
            }
        }
        
        .header {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            padding: 30px;
            text-align: center;
        }
        
        .header h1 {
            font-size: 28px;
            margin-bottom: 10px;
        }
        
        .header p {
            font-size: 14px;
            opacity: 0.9;
        }
        
        .content {
            padding: 30px;
        }
        
        .section {
            margin-bottom: 25px;
            border: 1px solid #e0e0e0;
            border-radius: 12px;
            overflow: hidden;
        }
        
        .section-title {
            background: #f5f5f5;
            padding: 15px 20px;
            font-weight: bold;
            color: #333;
            border-bottom: 1px solid #e0e0e0;
            font-size: 18px;
        }
        
        .section-title i {
            margin-right: 10px;
        }
        
        .section-body {
            padding: 20px;
        }
        
        .input-group {
            margin-bottom: 20px;
        }
        
        .input-group label {
            display: block;
            margin-bottom: 8px;
            font-weight: 600;
            color: #555;
            font-size: 14px;
        }
        
        .input-group input {
            width: 100%;
            padding: 12px 15px;
            border: 2px solid #e0e0e0;
            border-radius: 10px;
            font-size: 14px;
            transition: all 0.3s;
            font-family: monospace;
        }
        
        .input-group input:focus {
            outline: none;
            border-color: #667eea;
            box-shadow: 0 0 0 3px rgba(102, 126, 234, 0.1);
        }
        
        .input-group input[type="password"] {
            letter-spacing: 3px;
        }
        
        .bot-id-display {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            padding: 20px;
            border-radius: 12px;
            text-align: center;
            margin-bottom: 20px;
        }
        
        .bot-id-display .label {
            font-size: 12px;
            opacity: 0.9;
            margin-bottom: 10px;
        }
        
        .bot-id-display .value {
            font-size: 32px;
            font-weight: bold;
            letter-spacing: 2px;
            font-family: monospace;
        }
        
        .warning-box {
            background: #fff3cd;
            border-left: 4px solid #ffc107;
            padding: 12px 15px;
            border-radius: 8px;
            margin-bottom: 20px;
        }
        
        .warning-box i {
            color: #856404;
            font-weight: bold;
        }
        
        .info-box {
            background: #e3f2fd;
            border-left: 4px solid #2196f3;
            padding: 12px 15px;
            border-radius: 8px;
            margin-bottom: 20px;
            font-size: 13px;
        }
        
        button {
            width: 100%;
            padding: 14px;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            border: none;
            border-radius: 10px;
            font-size: 16px;
            font-weight: bold;
            cursor: pointer;
            transition: transform 0.2s, box-shadow 0.2s;
        }
        
        button:hover {
            transform: translateY(-2px);
            box-shadow: 0 5px 15px rgba(102, 126, 234, 0.4);
        }
        
        button:active {
            transform: translateY(0);
        }
        
        .status {
            margin-top: 20px;
            padding: 12px;
            border-radius: 8px;
            text-align: center;
            font-size: 13px;
            display: none;
        }
        
        .status.show {
            display: block;
        }
        
        .status.success {
            background: #d4edda;
            color: #155724;
            border: 1px solid #c3e6cb;
        }
        
        .status.error {
            background: #f8d7da;
            color: #721c24;
            border: 1px solid #f5c6cb;
        }
        
        .status.loading {
            background: #d1ecf1;
            color: #0c5460;
            border: 1px solid #bee5eb;
        }
        
        .footer {
            background: #f5f5f5;
            padding: 15px;
            text-align: center;
            font-size: 11px;
            color: #999;
        }
        
        .device-info {
            font-size: 12px;
            color: #666;
            text-align: center;
            margin-top: 15px;
        }
        
        @media (max-width: 480px) {
            .container {
                margin: 10px;
            }
            
            .content {
                padding: 20px;
            }
            
            .bot-id-display .value {
                font-size: 24px;
            }
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>⚡ Energy Meter Setup</h1>
            <p>Configure WiFi & Verify Bot ID</p>
        </div>
        
        <div class="content">
            <div class="bot-id-display">
                <div class="label">🔑 Required Bot ID</div>
                <div class="value">%BOT_ID%</div>
            </div>
            
            <div class="warning-box">
                <i>⚠️ IMPORTANT:</i> You must enter the exact Bot ID shown above. Mismatch will prevent MQTT communication.
            </div>
            
            <form id="configForm" action="/submit" method="POST">
                <div class="section">
                    <div class="section-title">
                        <i>📡</i> WiFi Network
                    </div>
                    <div class="section-body">
                        <div class="input-group">
                            <label>WiFi SSID</label>
                            <input type="text" id="ssid" name="ssid" placeholder="Enter WiFi name" required autocomplete="off">
                        </div>
                        <div class="input-group">
                            <label>WiFi Password</label>
                            <input type="password" id="password" name="password" placeholder="Enter WiFi password" required>
                        </div>
                    </div>
                </div>
                
                <div class="section">
                    <div class="section-title">
                        <i>🔐</i> Bot Verification
                    </div>
                    <div class="section-body">
                        <div class="input-group">
                            <label>Enter Bot ID</label>
                            <input type="text" id="bot_id" name="bot_id" placeholder="Enter %BOT_ID%" required autocomplete="off">
                        </div>
                    </div>
                </div>
                
                <button type="submit" id="submitBtn">
                    <span id="btnText">▶ CONNECT & VERIFY</span>
                    <span id="btnSpinner" style="display:none;">⏳ Connecting...</span>
                </button>
                
                <div id="statusMessage" class="status"></div>
                
                <div class="info-box">
                    <strong>ℹ️ What happens next:</strong><br>
                    1. Device connects to your WiFi<br>
                    2. Verifies the Bot ID<br>
                    3. Starts MQTT communication<br>
                    4. Begins publishing energy data
                </div>
            </form>
        </div>
        
        <div class="footer">
            <div>ESP32 Energy Monitor v1.0</div>
            <div class="device-info">Device will restart after successful configuration</div>
        </div>
    </div>
    
    <script>
        const form = document.getElementById('configForm');
        const submitBtn = document.getElementById('submitBtn');
        const btnText = document.getElementById('btnText');
        const btnSpinner = document.getElementById('btnSpinner');
        const statusDiv = document.getElementById('statusMessage');
        
        // Try to scan for available WiFi networks
        async function scanNetworks() {
            try {
                const response = await fetch('/scan');
                const networks = await response.json();
                if (networks && networks.length > 0) {
                    const ssidInput = document.getElementById('ssid');
                    const datalist = document.createElement('datalist');
                    datalist.id = 'wifi-list';
                    ssidInput.setAttribute('list', 'wifi-list');
                    
                    networks.forEach(network => {
                        const option = document.createElement('option');
                        option.value = network;
                        datalist.appendChild(option);
                    });
                    
                    document.body.appendChild(datalist);
                }
            } catch(e) {
                console.log('Network scan not available');
            }
        }
        
        // Auto-fill bot ID if empty
        const botIdDisplay = document.querySelector('.bot-id-display .value');
        const botIdInput = document.getElementById('bot_id');
        
        if (botIdDisplay && botIdInput) {
            const requiredBotId = botIdDisplay.innerText.trim();
            botIdInput.placeholder = `Enter ${requiredBotId}`;
        }
        
        form.addEventListener('submit', function(e) {
            e.preventDefault();
            
            // Show loading state
            btnText.style.display = 'none';
            btnSpinner.style.display = 'inline';
            submitBtn.disabled = true;
            
            // Clear previous status
            statusDiv.className = 'status';
            statusDiv.classList.remove('show');
            
            // Get form data
            const formData = new FormData(form);
            const ssid = formData.get('ssid');
            const password = formData.get('password');
            const botId = formData.get('bot_id');
            const requiredBotId = botIdDisplay.innerText.trim();
            
            // Validate inputs
            if (!ssid || ssid.length < 2) {
                showStatus('❌ Please enter a valid WiFi SSID', 'error');
                resetButton();
                return;
            }
            
            if (password.length < 8) {
                showStatus('⚠️ WiFi password should be at least 8 characters', 'error');
                resetButton();
                return;
            }
            
            if (!botId) {
                showStatus('❌ Please enter the Bot ID', 'error');
                resetButton();
                return;
            }
            
            if (botId !== requiredBotId) {
                showStatus(`❌ Invalid Bot ID! Expected: ${requiredBotId}`, 'error');
                resetButton();
                return;
            }
            
            // Show connecting status
            showStatus('📡 Connecting to WiFi and verifying Bot ID...', 'loading');
            
            // Send data via AJAX
            fetch('/submit', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded',
                },
                body: new URLSearchParams(formData)
            })
            .then(response => response.text())
            .then(data => {
                if (data.includes('SUCCESS') || data.includes('success')) {
                    showStatus('✅ Success! Device is connecting... Page will refresh.', 'success');
                    setTimeout(() => {
                        window.location.href = '/success';
                    }, 2000);
                } else if (data.includes('ERROR')) {
                    showStatus('❌ Connection failed. Please check your credentials.', 'error');
                    resetButton();
                } else {
                    showStatus('⚠️ Unexpected response. Trying normal submission...', 'loading');
                    form.submit();
                }
            })
            .catch(error => {
                // If AJAX fails, submit normally
                form.submit();
            });
            
            // Timeout fallback
            setTimeout(() => {
                if (submitBtn.disabled) {
                    showStatus('⚠️ Taking longer than expected. If this persists, check your credentials.', 'error');
                    resetButton();
                }
            }, 15000);
        });
        
        function showStatus(message, type) {
            statusDiv.textContent = message;
            statusDiv.className = `status ${type} show`;
        }
        
        function resetButton() {
            btnText.style.display = 'inline';
            btnSpinner.style.display = 'none';
            submitBtn.disabled = false;
        }
        
        // Auto-focus on SSID field
        document.getElementById('ssid').focus();
        
        // Try to scan networks on page load
        // scanNetworks();
    </script>
</body>
</html>
)rawliteral";

// ========== Success Page HTML ==========
const char successPageHTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta http-equiv='refresh' content='3;url=/'>
    <meta name='viewport' content='width=device-width, initial-scale=1'>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
            padding: 20px;
        }
        
        .container {
            background: white;
            border-radius: 20px;
            padding: 50px;
            max-width: 450px;
            text-align: center;
            box-shadow: 0 20px 60px rgba(0,0,0,0.3);
            animation: slideIn 0.5s ease-out;
        }
        
        @keyframes slideIn {
            from {
                opacity: 0;
                transform: scale(0.9);
            }
            to {
                opacity: 1;
                transform: scale(1);
            }
        }
        
        .success-icon {
            width: 80px;
            height: 80px;
            background: #4caf50;
            border-radius: 50%;
            display: flex;
            align-items: center;
            justify-content: center;
            margin: 0 auto 20px;
            animation: bounce 0.5s ease-out;
        }
        
        @keyframes bounce {
            0%, 100% { transform: scale(0); }
            50% { transform: scale(1.2); }
            70% { transform: scale(0.9); }
            100% { transform: scale(1); }
        }
        
        .success-icon span {
            font-size: 50px;
            color: white;
        }
        
        h2 {
            color: #333;
            margin-bottom: 10px;
        }
        
        .message {
            color: #666;
            margin: 20px 0;
            line-height: 1.6;
        }
        
        .info {
            background: #e3f2fd;
            padding: 15px;
            border-radius: 10px;
            margin-top: 20px;
            font-size: 14px;
            color: #1976d2;
        }
        
        .loader {
            width: 40px;
            height: 40px;
            border: 4px solid #f3f3f3;
            border-top: 4px solid #667eea;
            border-radius: 50%;
            animation: spin 1s linear infinite;
            margin: 20px auto;
        }
        
        @keyframes spin {
            0% { transform: rotate(0deg); }
            100% { transform: rotate(360deg); }
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="success-icon">
            <span>✓</span>
        </div>
        <h2>Configuration Successful!</h2>
        <div class="message">
            ✅ Bot ID Verified<br>
            📡 WiFi Connected<br>
            🔌 MQTT Communication Active
        </div>
        <div class="loader"></div>
        <div class="info">
            ⚡ Device is now publishing energy data to MQTT broker<br>
            Data will be available under topics with Bot ID: <strong>%BOT_ID%</strong>
        </div>
    </div>
</body>
</html>
)rawliteral";

// ========== Error Page HTML ==========
const char errorPageHTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta http-equiv='refresh' content='5;url=/'>
    <meta name='viewport' content='width=device-width, initial-scale=1'>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
            padding: 20px;
        }
        
        .container {
            background: white;
            border-radius: 20px;
            padding: 50px;
            max-width: 450px;
            text-align: center;
            box-shadow: 0 20px 60px rgba(0,0,0,0.3);
            animation: shake 0.5s ease-out;
        }
        
        @keyframes shake {
            0%, 100% { transform: translateX(0); }
            25% { transform: translateX(-10px); }
            75% { transform: translateX(10px); }
        }
        
        .error-icon {
            width: 80px;
            height: 80px;
            background: #f44336;
            border-radius: 50%;
            display: flex;
            align-items: center;
            justify-content: center;
            margin: 0 auto 20px;
        }
        
        .error-icon span {
            font-size: 50px;
            color: white;
        }
        
        h2 {
            color: #333;
            margin-bottom: 10px;
        }
        
        .message {
            color: #666;
            margin: 20px 0;
            line-height: 1.6;
        }
        
        .expected-id {
            background: #f5f5f5;
            padding: 15px;
            border-radius: 10px;
            margin: 15px 0;
            font-family: monospace;
            font-size: 24px;
            font-weight: bold;
            color: #667eea;
        }
        
        .info {
            background: #fff3cd;
            padding: 15px;
            border-radius: 10px;
            margin-top: 20px;
            font-size: 14px;
            color: #856404;
        }
        
        .countdown {
            margin-top: 15px;
            font-size: 12px;
            color: #999;
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="error-icon">
            <span>✗</span>
        </div>
        <h2>Configuration Failed!</h2>
        <div class="message">
            ❌ Invalid Bot ID entered<br>
            ❌ Or WiFi connection failed
        </div>
        <div class="expected-id">
            Expected: %BOT_ID%
        </div>
        <div class="info">
            ⚠️ Please enter the EXACT Bot ID shown above<br>
            Make sure WiFi credentials are correct
        </div>
        <div class="countdown">
            Redirecting to setup page...
        </div>
    </div>
</body>
</html>
)rawliteral";

// ========== Function to replace placeholder in HTML ==========
String processHTML(const char* html, String botID) {
    String result = String(html);
    result.replace("%BOT_ID%", botID);
    return result;
}

#endif