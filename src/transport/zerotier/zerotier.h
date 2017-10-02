//
// Copyright 2017 Garrett D'Amore <garrett@damore.org>
// Copyright 2017 Capitar IT Group BV <info@capitar.com>
//
// This software is supplied under the terms of the MIT License, a
// copy of which should be located in the distribution where this
// file was obtained (LICENSE.txt).  A copy of the license may also be
// found online at https://opensource.org/licenses/MIT.
//

#ifndef NNG_TRANSPORT_ZEROTIER_ZEROTIER_H
#define NNG_TRANSPORT_ZEROTIER_ZEROTIER_H

// ZeroTier Transport.  This sits on the ZeroTier L2 network, which itself
// is implemented on top of UDP.  This requires the 3rd party
// libzerotiercore library (which is GPLv3!) and platform specific UDP
// functionality to be built in.  Note that care must be taken to link
// dynamically if one wishes to avoid making your entire application GPL3.
// (Alternatively ZeroTier offers commercial licenses which may prevent
// this particular problem.)  This implementation does not make use of
// certain advanced capabilities in ZeroTier such as more sophisticated
// route management and TCP fallback.  You need to have connectivity
// to the Internet to use this.  (Or at least to your Planetary root.)
//
// The ZeroTier URL format we support is zt://<nwid>/<ztid>:<port> where
// the <nwid> component represents the 64-bit hexadecimal ZeroTier
// network ID,the <ztid> represents the 40-bit hexadecimal ZeroTier
// node (device) ID, and the <port> is a 24-bit (decimal) port number.
//
// A listener may elide the <ztid>/ portion, to just bind to itself,
// in which case the format will be zt://<nwid>:<port>
//
// A listener may also use either 0 or * for the <port> to indicate that
// a random local ephemeral port should be used.
//
// Because ZeroTier takes a while to establish connectivity, it is even
// more important that applications using the ZeroTier transport not
// assume that a connection will be immediately available.  It can take
// quite a few seconds for peer-to-peer connectivity to be established.
//
// The ZeroTier transport was funded by Capitar IT Group, BV.
//
// This transport is highly experimental.

// ZeroTier transport-specific options.

// NNG_OPT_ZT_HOME is a string containing a directory, where persistent
// state (key files, etc.) will be stored.  It should be protected from
// unauthorized viewing and modification.  This option must be set on an
// endpoint or socket before the endpoint(s) are started.  If the unset,
// or an empty string, then no persistence is used and an ephemeral node
// will be created instead.  Note that different endpoints may use different
// values for this option, and that will lead to each endpoint having a
// different ZeroTier identity -- however only one ephemeral node will
// be created for the application.
#define NNG_OPT_ZT_HOME "zt:home"

// NNG_OPT_ZT_NWID is the 64-bit network ID, represented using a uint64_t in
// native byte order.  This is a read-only option; it is derived automatically
// from the URL.
#define NNG_OPT_ZT_NWID "zt:nwid"

// NNG_OPT_ZT_NODE is the 40-bit node ID, stored in native order in the low
// 40-bits of a uint64_t, of the node.  This is a read-only option.
#define NNG_OPT_ZT_NODE "zt:node"

// NNG_OPT_ZT_NETWORK_STATUS represents the status of the ZeroTier virtual
// network.  The option is a read-only value, stored as an integer, which
// takes of the nng_zt_network_status_xxx values listed below.
#define NNG_OPT_ZT_NETWORK_STATUS "zt:network-status"

// NNG_OPT_ZT_NETWORK_NAME is a human-readable name for the ZeroTier virtual
// network.  This will only be set once the ZeroTier network has come up
// as the name comes from the network controller.  This is read-only, and
// is presented as an ASCIIZ string.
#define NNG_OPT_ZT_NETWORK_NAME "zt:network-name"

// NNG_OPT_ZT_PING_TIME and NNG_OPT_ZT_PING_COUNT are used to send ping
// requests when a connection appears to be idled.  If a logical session
// has not received traffic from it's peer for ping-time, then a ping packet
// is sent.  This will be done up to ping-count times.  If no traffic from
// the remote peer is seen after all ping requests are sent, then the peer
// is assumed to be dead or offline, and the session is closed.  The
// NNG_OPT_ZT_PING_TIME is a duration (usec, stored as an nng_duration, and
// NNG_OPT_ZT_PING_COUNT is an integer.)  This ping process can be disabled
// by setting either ping-time or ping-count to zero.
#define NNG_OPT_ZT_PING_TIME "zt:ping-time"
#define NNG_OPT_ZT_PING_COUNT "zt:ping-count"

// NNG_OPT_ZT_MTU is a read-only size_t and contains the ZeroTier virtual
// network MTU (i.e. the L2 payload MTU). Messages that are larger than this
// (including our 20-byte header data) will be fragmented into multiple
// virtual L2 frames.
#define NNG_OPT_ZT_MTU "zt:mtu"

// Network status values.
// These values are supplied to help folks checking status.  They are the
// return values from zt_opt_status.  We avoid hard coding them as defines,
// to keep applications from baking in values that may change if the
// underlying ZeroTier transport changes.
extern int nng_zt_network_status_configuring;
extern int nng_zt_network_status_ok;
extern int nng_zt_network_status_denied;
extern int nng_zt_network_status_notfound;
extern int nng_zt_network_status_error;
extern int nng_zt_network_status_obsolete;

extern int nng_zt_register(void);

#endif // NNG_TRANSPORT_ZEROTIER_ZEROTIER_H
