package com.shiro.sosapp

import android.content.Intent
import android.net.Uri
import android.os.Bundle
import android.widget.Button
import android.widget.EditText
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity

class MainActivity : AppCompatActivity() {
    companion object {
        init {
            System.loadLibrary("soscore")
        }
    }

    external fun buildSosPayload(name: String, latitude: Double, longitude: Double): String
    external fun severityScore(payload: String): Int

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val nameInput = findViewById<EditText>(R.id.nameInput)
        val latInput = findViewById<EditText>(R.id.latInput)
        val lonInput = findViewById<EditText>(R.id.lonInput)
        val phoneInput = findViewById<EditText>(R.id.phoneInput)
        val previewText = findViewById<TextView>(R.id.previewText)
        val previewBtn = findViewById<Button>(R.id.previewBtn)
        val sendBtn = findViewById<Button>(R.id.sendBtn)

        fun buildPayload(): String {
            val name = nameInput.text.toString().ifBlank { "Unknown User" }
            val lat = latInput.text.toString().toDoubleOrNull() ?: 0.0
            val lon = lonInput.text.toString().toDoubleOrNull() ?: 0.0
            return buildSosPayload(name, lat, lon)
        }

        previewBtn.setOnClickListener {
            val payload = buildPayload()
            val score = severityScore(payload)
            previewText.text = "Severity: $score/100\n\n$payload"
        }

        sendBtn.setOnClickListener {
            val number = phoneInput.text.toString().trim()
            val payload = buildPayload()
            val smsUri = Uri.parse("smsto:$number")
            val intent = Intent(Intent.ACTION_SENDTO, smsUri).apply {
                putExtra("sms_body", payload)
            }
            startActivity(intent)
        }
    }
}
