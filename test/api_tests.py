import unittest
import requests
import json

host = "192.168.1.157"       # Replace these with the IP of your device and ID of your device.
id = "7376ef"

#
# Tests for checking that the device API works as planned 
#
def call_set_config( param, value ):
    url = 'http://' + host + '/api/config/set?'
    headers = { 'ContentType': 'text/plain' }
    return requests.get( url + "id=" + id + "&param=" + param + "&value=" + value, headers=headers, data="")

def call_get_config( param ):
    url = 'http://' + host + '/api/config/get?'
    headers = { 'ContentType': 'text/plain' }
    return requests.get( url + "param=" + param, headers=headers, data="")

class ApiTests(unittest.TestCase):
    
    def test_setmdns(self):
        resp = call_set_config( "mdns", "myvalue")
        self.assertEqual(resp.status_code, 200, "Should be 200")
        resp = call_get_config( "mdns" )
        self.assertEqual( resp.json()["mdns"], "myvalue", "Wrong response." )

    def test_otaurl(self):
        resp = call_set_config( "otaurl", "myvalue1")
        self.assertEqual(resp.status_code, 200, "Should be 200")
        resp = call_get_config( "otaurl" )
        self.assertEqual( resp.json()["otaurl"], "myvalue1", "Wrong response." )

    def test_temp1(self):
        resp = call_set_config( "tempformat", "C")
        self.assertEqual(resp.status_code, 200, "Should be 200")
        resp = call_set_config( "TEMPFORMAT", "C")
        self.assertEqual(resp.status_code, 200, "Should be 200")
        resp = call_get_config( "tempformat" )
        self.assertEqual( resp.json()["tempformat"], "C", "Wrong response." )

    def test_temp2(self):
        resp = call_set_config( "tempformat", "F")
        self.assertEqual(resp.status_code, 200, "Should be 200")
        resp = call_get_config( "tempformat" )
        self.assertEqual( resp.json()["tempformat"], "F", "Wrong response." )

    def test_temp3(self):
        resp = call_set_config( "tempformat", "Q")
        self.assertEqual(resp.status_code, 400, "Should be 400")

    def test_httppush(self):
        resp = call_set_config( "httppush", "myvalue4")
        self.assertEqual(resp.status_code, 200, "Should be 200")
        resp = call_get_config( "httppush" )
        self.assertEqual( resp.json()["httppush"], "myvalue4", "Wrong response." )

    def test_fermentrackpush(self):
        resp = call_set_config( "fermentrackpush", "myvalue2")
        self.assertEqual(resp.status_code, 200, "Should be 200")
        resp = call_get_config( "fermentrackpush" )
        self.assertEqual( resp.json()["fermentrackpush"], "myvalue2", "Wrong response." )

    def test_brewfatherpush(self):
        resp = call_set_config( "brewfatherpush", "myvalue3")
        self.assertEqual(resp.status_code, 200, "Should be 200")
        resp = call_get_config( "brewfatherpush" )
        self.assertEqual( resp.json()["brewfatherpush"], "myvalue3", "Wrong response." )

    def test_pressurecorr(self):
        resp = call_set_config( "pressurecorr", "1.23")
        self.assertEqual(resp.status_code, 200, "Should be 200")
        resp = call_get_config( "pressurecorr" )
        self.assertEqual( resp.json()["pressurecorr"], 1.23, "Wrong response." )

    def test_voltagefactor(self):
        resp = call_set_config( "voltagefactor", "1.42")
        self.assertEqual(resp.status_code, 200, "Should be 200")
        resp = call_get_config( "voltagefactor" )
        self.assertEqual( resp.json()["voltagefactor"], 1.42, "Wrong response." )

if __name__ == '__main__':
    unittest.main()

