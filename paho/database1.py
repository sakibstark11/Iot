import numpy
import json
import array




def read_from_db():
    with open('datajson.json') as f:
        data = json.load(f)
        names = [sensors['sensor_id'] for sensors in data['sensor_data']]
        states = [sensors['sensor_state'] for sensors in data['sensor_data']]
        print (names)
        return names , states
