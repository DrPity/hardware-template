#!/bin/sh
mac2=$(( $RANDOM % 256 ))
mac3=$(( $RANDOM % 256 ))
mac4=$(( $RANDOM % 256 ))
mac5=$(( $RANDOM % 256 ))
mac6=$(( $RANDOM % 256 ))

printf -- '-DMAC2=%u -DMAC3=%u -DMAC4=%u -DMAC5=%u -DMAC6=%u'  $mac2 $mac3 $mac4 $mac5 $mac6
