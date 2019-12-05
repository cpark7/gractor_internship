#!/bin/bash

exec mosquitto_pub -t "/cmd/door/minki" -m "1" -h "nas.gractor.com" -u "gractor" -P "gractor1234"
