#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <Adafruit_NeoPixel.h>

// Configuration
#define PIEZO_PIN 34          // Analog pin connected to piezo sensor
#define LED_PIN 13             // Built-in LED for status indication
#define PIXEL_PIN 15           // Pin for NeoPixel
#define NUM_PIXELS 8           // Number of NeoPixels
#define SAMPLE_WINDOW 50       // Sample window width in mS (50 mS = 20Hz)
#define SAMPLE_RATE 10000      // Sample rate in Hz
#define SAMPLES 512            // Must be a power of 2
#define THRESHOLD 500          // Minimum amplitude to trigger detection

// Frequency bands for analysis (Hz)
const float BANDS[] = {32, 64, 125, 250, 500, 1000, 2000, 4000, 8000, 16000};
const int NUM_BANDS = sizeof(BANDS) / sizeof(BANDS[0]);

// Global variables
Adafruit_NeoPixel pixels(NUM_PIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
WebServer server(80);

// FFT variables
unsigned long sampling_period_us;
unsigned long microseconds;
double vReal[SAMPLES];
double vImag[SAMPLES];

// Energy storage
float energyLevel = 0.0;
float maxEnergy = 0.0;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  // Initialize NeoPixels
  pixels.begin();
  pixels.show(); // Initialize all pixels to 'off'
  
  // Set up pins
  pinMode(LED_PIN, OUTPUT);
  pinMode(PIEZO_PIN, INPUT);
  
  // Calculate sampling period
  sampling_period_us = round(1000000 * (1.0 / SAMPLE_RATE));
  
  // Initialize WiFi (optional, for future OTA updates)
  // setupWiFi();
  
  // Setup web server endpoints
  setupWebServer();
  
  // Initial status
  Serial.println("VoxLux Sound Harvester initialized");
  blinkLED(3, 200);
}

void loop() {
  // Handle web server requests
  server.handleClient();
  
  // Sample sound and process FFT
  sampleSound();
  
  // Update energy level visualization
  updateVisualization();
  
  // Small delay to prevent watchdog reset
  delay(10);
}

void sampleSound() {
  // Sample the piezo sensor
  unsigned long startMillis = millis();
  unsigned int sample;
  
  // Collect samples for the sample window
  for (int i = 0; i < SAMPLES; i++) {
    microseconds = micros();
    
    sample = analogRead(PIEZO_PIN);
    vReal[i] = sample;         // Store the value from the piezo in vReal
    vImag[i] = 0;              // Imaginary part must be zeroed for FFT
    
    // Wait for next sample period
    while (micros() < (microseconds + sampling_period_us)) {
      // Busy wait
    }
  }
  
  // Perform FFT
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
  
  // Analyze frequency bands
  for (int i = 2; i < (SAMPLES/2); i++) { // Skip DC and first frequency
    if (vReal[i] > THRESHOLD) {
      // Calculate frequency of this bin
      float frequency = (i * 1.0 * SAMPLE_RATE) / SAMPLES;
      
      // Map to our frequency bands
      for (int j = 0; j < NUM_BANDS - 1; j++) {
        if (frequency >= BANDS[j] && frequency < BANDS[j+1]) {
          // Add to energy level based on amplitude and frequency
          energyLevel += vReal[i] * (j + 1) * 0.1;
          break;
        }
      }
    }
  }
  
  // Decay energy level over time
  energyLevel = max(0.0, energyLevel * 0.95);
  maxEnergy = max(maxEnergy, energyLevel);
}

void updateVisualization() {
  // Map energy level to NeoPixel brightness
  int brightness = map(constrain(energyLevel, 0, maxEnergy), 0, maxEnergy, 0, 255);
  
  // Update all pixels
  for (int i = 0; i < NUM_PIXELS; i++) {
    // Create a color gradient from deep violet to electric blue
    uint8_t r = map(i, 0, NUM_PIXELS - 1, 76, 0);     // Deep violet to black
    uint8_t g = map(i, 0, NUM_PIXELS - 1, 0, 100);    // Black to blue-green
    uint8_t b = map(i, 0, NUM_PIXELS - 1, 153, 255);  // Deep violet to electric blue
    
    // Scale color by brightness
    r = (r * brightness) / 255;
    g = (g * brightness) / 255;
    b = (b * brightness) / 255;
    
    pixels.setPixelColor(i, pixels.Color(r, g, b));
  }
  
  // Update the pixels
  pixels.show();
  
  // Update status LED
  digitalWrite(LED_PIN, brightness > 10 ? HIGH : LOW);
}

void setupWebServer() {
  // Root endpoint
  server.on("/", HTTP_GET, []() {
    String html = "<html><head><title>VoxLux Sound Harvester</title></head>";
    html += "<body style='background: #0f0f1a; color: #e0e0ff; font-family: monospace;'>";
    html += "<h1>VoxLux Sound Harvester</h1>";
    html += "<p>Energy Level: <span id='energy'>" + String(energyLevel) + "</span></p>";
    html += "<p>Max Energy: <span id='max'>" + String(maxEnergy) + "</span></p>";
    html += "<script>setInterval(updateData, 1000);";
    html += "function updateData(){fetch('/data').then(r=>r.json()).then(d=>{";
    html += "document.getElementById('energy').innerText=d.energy; ";
    html += "document.getElementById('max').innerText=d.max;})}</script>";
    html += "</body></html>";
    
    server.send(200, "text/html", html);
  });
  
  // Data endpoint
  server.on("/data", HTTP_GET, []() {
    StaticJsonDocument<200> doc;
    doc["energy"] = energyLevel;
    doc["max"] = maxEnergy;
    
    String response;
    serializeJson(doc, response);
    server.send(200, "application/json", response);
  });
  
  // Start server
  server.begin();
}

void blinkLED(int count, int delayTime) {
  for (int i = 0; i < count; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(delayTime);
    digitalWrite(LED_PIN, LOW);
    if (i < count - 1) delay(delayTime);
  }
}

// Placeholder for FFT functions (to be implemented or use a library)
class FFT {
public:
  static void Windowing(double *vData, uint16_t samples, uint8_t windowType, uint8_t dir) {
    // Implementation of windowing function
  }
  
  static void Compute(double *vReal, double *vImag, uint16_t samples, uint8_t dir) {
    // Implementation of FFT computation
  }
  
  static void ComplexToMagnitude(double *vReal, double *vImag, uint16_t samples) {
    // Implementation of complex to magnitude conversion
  }
};
