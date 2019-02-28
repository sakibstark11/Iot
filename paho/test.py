
#!/usr/bin/env python

import paho.mqtt.client as mqtt  #import the client
import time
from datetime import datetime
import json
import os
import sys
def read_from_db():
   
    with open('D:/EspDev2/Gitting/lion/nodeweb/datajson.json') as f:
        data = json.load(f)
        names = [sensors['sensor_id'] for sensors in data['sensor_data']]
        states = [sensors['sensor_state'] for sensors in data['sensor_data']]
        sensitivity = [sensors['sensor_sensitivity'] for sensors in data['sensor_data']]
        threshold = [sensors['sensor_threshold'] for sensors in data['sensor_data']]
        return names , states , sensitivity , threshold

mqtt_cert =  "D:/EspDev2/Gitting/lion/paho/mqtt.crt"
broker="mqtt.coventry.ac.uk"
client = mqtt.Client("lion")
client.username_pw_set("302CEM", "n3fXXFZrjw")
client.tls_set(mqtt_cert)
port = 8883

def publish():
   _,sensor_message,_,_ = read_from_db()
   sensor_topic, _,_,_ = read_from_db()
   _,_,sensor_sensitivity,_ = read_from_db()
   _,_,_,sensor_threshold = read_from_db()
   time.sleep(0)
   client.publish(sensor_topic[0], "test+hhhhh", qos=0, retain=False)
   time.sleep(0)
   #client.publish(sensor_topic[0], sensor_sensitivity[0], qos=0, retain=False)
   #time.sleep(1)
   #client.publish(sensor_topic[0], sensor_threshold[0], qos=0, retain=False)
   time.sleep(0.5)


def on_subscribe(client, userdata, message):   #create function for callback
   print("subscribed with qo")
   pass

def on_connect(client, userdata, flags, rc):
    if rc==0:
        client.connected_flag=True #set flag
        print("connected OK")
    else:
        print("Bad connection Returned code=",rc)

def on_message(client, userdata, message):
    print("message received  "  ,str(message.payload.decode("utf-8")))

def on_publish(client,userdata,mid):   #create function for callback
   print("data published mid=",mid, "\n")
   pass

def on_disconnect(client, userdata, rc):
   print("client disconnected ok") 

mqtt.Client.connected_flag=False #create flag in class

client.on_connect=on_connect
client.on_message=on_message 
client.on_publish=on_publish
client.on_disconnect = on_disconnect
client.on_subscribe = on_subscribe
client.loop_start()
print("Connecting to broker ",broker)
client.connect(broker, port)
while not client.connected_flag: #wait in loop
   print("In wait loop")
   time.sleep(1)
print("in Main Loop")
while(1):

   publish()
new_mod = (os.path.getmtime('D:/EspDev2/Gitting/lion/nodeweb/datajson.json')),datetime.fromtimestamp(os.path.getmtime('D:/EspDev2/Gitting/lion/nodeweb/datajson.json')) #checking for changes
while(1):
   previous_mod = (os.path.getmtime('D:/EspDev2/Gitting/lion/nodeweb/datajson.json')),datetime.fromtimestamp(os.path.getmtime('D:/EspDev2/Gitting/lion/nodeweb/datajson.json'))
   if(new_mod != previous_mod):
      print ("changes")
      publish() # publishing the desired data
      
      new_mod = (os.path.getmtime('D:/EspDev2/Gitting/lion/nodeweb/datajson.json')),datetime.fromtimestamp(os.path.getmtime('D:/EspDev2/Gitting/lion/nodeweb/datajson.json'))
   else:
      print("", end="")
      
      



   
