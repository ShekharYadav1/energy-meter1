#ifndef HTMLFILE_H
#define HTMLFILE_H

// ========== Bot ID Verification Webpage HTML ==========
const char botIDPageHTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta name='viewport' content='width=device-width, initial-scale=1'>
    <style>
        body {
            font-family: 'Segoe UI', Arial, sans-serif;
            text-align: center;
            margin-top: 80px;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
        }
        .container {
            background: white;
            padding: 40px;
            border-radius: 20px;
            max-width: 400px;
            margin: auto;
            box-shadow: 0 20px 60px rgba(0,0,0,0.3);
        }
        h1 {
            color: #333;
            margin-bottom: 10px;
        }
        .bot-id-box {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            padding: 25px;
            border-radius: 15px;
            margin: 20px 0;
            font-size: 56px;
            font-weight: bold;
        }
        .warning {
            background: #fff3cd;
            color: #856404;
            padding: 12px;
            border-radius: 10px;
            margin: 20px 0;
        }
        input {
            padding: 14px;
            margin: 15px 0;
            width: 250px;
            font-size: 18px;
            border: 2px solid #ddd;
            border-radius: 10px;
            text-align: center;
        }
        button {
            padding: 14px 35px;
            background: #4CAF50;
            color: white;
            border: none;
            border-radius: 10px;
            cursor: pointer;
            font-size: 16px;
        }
        button:hover {
            background: #45a049;
        }
        .footer {
            margin-top: 20px;
            color: #666;
            font-size: 12px;
        }
    </style>
</head>
<body>
    <div class='container'>
        <h1>🔑 Bot ID Required</h1>
        <div class='bot-id-box'>%BOT_ID%</div>
        <div class='warning'>
            ⚠️ Incorrect Bot ID was entered before!
        </div>
        <h3>Enter the correct Bot ID:</h3>
        <form action='/submit' method='POST'>
            <input type='text' name='bot_id' placeholder='Enter %BOT_ID%' required autocomplete='off'>
            <br>
            <button type='submit'>▶ VERIFY & START</button>
        </form>
        <div class='footer'>Device expects: <strong>%BOT_ID%</strong></div>
    </div>
</body>
</html>
)rawliteral";

// ========== Success Page HTML ==========
const char successPageHTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta http-equiv='refresh' content='2;url=/success'>
    <style>
        body {
            font-family: Arial;
            text-align: center;
            margin-top: 50px;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
        }
        .container {
            background: white;
            padding: 40px;
            border-radius: 20px;
            max-width: 400px;
            margin: auto;
            box-shadow: 0 20px 60px rgba(0,0,0,0.3);
        }
        h2 {
            color: green;
        }
    </style>
</head>
<body>
    <div class='container'>
        <h2>✅ Bot ID MATCHED!</h2>
        <p>MQTT Communication Starting...</p>
        <p>Data will be published to MQTT broker</p>
    </div>
</body>
</html>
)rawliteral";

// ========== Error Page HTML ==========
const char errorPageHTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta http-equiv='refresh' content='3;url=/'>
    <style>
        body {
            font-family: Arial;
            text-align: center;
            margin-top: 50px;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
        }
        .container {
            background: white;
            padding: 40px;
            border-radius: 20px;
            max-width: 400px;
            margin: auto;
            box-shadow: 0 20px 60px rgba(0,0,0,0.3);
        }
        .error {
            color: red;
            background: #ffebee;
            padding: 20px;
            border-radius: 10px;
        }
    </style>
</head>
<body>
    <div class='container'>
        <div class='error'>
            <h2>❌ Invalid Bot ID!</h2>
            <p>Expected: <strong>%BOT_ID%</strong></p>
            <p>Redirecting to try again...</p>
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