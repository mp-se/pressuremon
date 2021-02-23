import requests
import json

host = "192.168.1.10"       # Replace these with the IP of your device and ID of your device.
id = "7376ef"

def do_set_config( param, value ):

    url = 'http://' + host + '/api/config/set?'
    headers = { 'ContentType': 'text/plain' }


    resp = requests.get( url + "id=" + id + "&param=" + param + "&value=" + value, headers=headers, data="")

    if resp.status_code != 200 :
        print ( "Failed to set " + param )
    else :
        print ( "Success for " + param )


def do_config( ):

    url = 'http://' + host + '/config'
    headers = { 'ContentType': 'text/plain' }

    resp = requests.get( url, headers=headers, data="")

    if resp.status_code != 200 :
        print ( "Failed to get config" )
    else :
        print ( resp.json() )


cfg_empty           = ""
cfg_mdns            = "test"
cfg_otaurl          = "http://mywebserver/firmware/pressmon/"
cfg_fermentrackpush = "http://myfermentrack/ispindel"
cfg_brewfatherpush  = ""
cfg_pushinterval    = "60"
cfg_tempformat      = "C"
cfg_pressurecorr    = "0.0"
cfg_voltagefactor   = "1.63"
cfg_httppush        = "http://mylogger:9090/api/v1/qebfiZIyHccYtTV1ITiy/telemetry"

do_set_config( "mdns", cfg_mdns )
do_set_config( "fermentrackpush", cfg_fermentrackpush )
do_set_config( "brewfatherpush", cfg_brewfatherpush )
do_set_config( "httppush", cfg_httppush )
do_set_config( "pushinterval", cfg_pushinterval )
do_set_config( "pressurecorr", cfg_pressurecorr )
do_set_config( "tempformat", cfg_tempformat )

do_config()
