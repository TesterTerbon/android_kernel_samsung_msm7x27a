#
# Makefile for the Linux TIPC layer
#

obj-$(CONFIG_TIPC) := tipc.o

tipc-y	+= addr.o bcast.o bearer.o config.o \
	   core.o handler.o link.o discover.o msg.o  \
	   name_distr.o  subscr.o name_table.o net.o  \
	   netlink.o node.o node_subscr.o port.o ref.o  \
<<<<<<< HEAD
	   socket.o log.o eth_media.o

# End of file
=======
	   socket.o log.o eth_media.o server.o

tipc-$(CONFIG_TIPC_MEDIA_IB)	+= ib_media.o
tipc-$(CONFIG_SYSCTL)		+= sysctl.o
>>>>>>> refs/remotes/origin/master
