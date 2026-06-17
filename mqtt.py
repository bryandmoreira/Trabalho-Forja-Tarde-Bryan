import speech_recognition as sr
import paho.mqtt.client as mqtt
import json
import time

# MQTT
BROKER = "broker.hivemq.com"
PORT = 1883
TOPIC = "forja/desenvolvimento/tarde"
client = mqtt.Client()
client.connect(BROKER, PORT, 60)
client.loop_start()

# Voz
recognizer = sr.Recognizer()
mic = sr.Microphone()
#with mic as source:
#  print("Calibrando...")
#  recognizer.adjust_for_ambient_noise(source, duration=2)

print("Pronto! Mande o comando:\n")
while True:
  try:
    with mic as source:
      print("Ouvindo...")
      # Captura o audio
      audio = recognizer.listen(
        source,
        timeout=5,
        phrase_time_limit=5
      )
    
    # Transforma em texto
    texto = recognizer.recognize_google(
      audio,
      language="pt-BR"
    )
    # Remove espaços no começo e final da frase
    texto = texto.strip()
    print("Você disse: " + texto)

    # Separar texto em comando e valor
    partes = texto.split(maxsplit=1)
    comando = partes[0]
    valor = partes[1] if len(partes) > 1 else ""

    # Enviar por MQTT
    payload = {
      "command": comando,
      "value": valor.capitalize()
    }
    mensagem = json.dumps(
      payload,
      ensure_ascii=False
    )
    client.publish(TOPIC, mensagem)
    print("Enviado!")

  except sr.UnknownValueError:
    print("Não entendi")

  except Exception as e:
    print("Erro:", e)

  time.sleep(1)