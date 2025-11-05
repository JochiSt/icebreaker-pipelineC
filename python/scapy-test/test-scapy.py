import scapy.all as scapy

# packet = scapy.Ether() / scapy.IP() / scapy.ICMP()
# # print raw packet
# print(scapy.raw(packet))

# print(packet.sprintf("%Ether.src% > %Ether.dst%\n%IP.src% > %IP.dst%"))

# packet.show()

################################################################################
# ARP packet

arp_packet = scapy.Ether() / scapy.ARP(pdst="192.168.0.20")

print(str(scapy.raw(arp_packet)))


