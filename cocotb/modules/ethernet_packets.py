# the following function is heavily inspired by
# https://www.codeproject.com/Tips/612847/Generate-a-quick-and-easy-custom-pcap-file-using-P

# create UDP data to be transmitted via cocotb ethernet
import struct

class Ethernet_packet(object):
    IPv4_DATAGRAM = 0x0800
    ARP = 0x0806

    ETH_HEADER =   ('SM SM SM SM SM SM'     #Source Mac
                    'DM DM DM DM DM DM'     #Dest Mac
                    'PP PP')                #Protocol (0x0800 = IP)

    def __init__(self, SRC_MAC, DST_MAC, protocol=IPv4_DATAGRAM):
        self.bytes = self.ETH_HEADER
        self.bytes = self.bytes.replace('SM SM SM SM SM SM', "%06x"%(SRC_MAC))
        self.bytes = self.bytes.replace('DM DM DM DM DM DM', "%06x"%(DST_MAC))
        self.bytes = self.bytes.replace('PP PP', "%04x"%(protocol))
        print("created Ethernet Header")
        print("\t\t" + self.bytes)

    def print(self):
        print(self.bytes)

    def payload(self):
        return bytearray(self.bytes)

    @staticmethod
    def getByteLength(str1):
        return int(len(''.join(str1.split())) / 2)

    def write_pcap(self, file_name):
        #Global header for pcap 2.4
        pcap_global_header =   ('D4 C3 B2 A1'
                                '02 00'         #File format major revision (i.e. pcap <2>.4)
                                '04 00'         #File format minor revision (i.e. pcap 2.<4>)
                                '00 00 00 00'
                                '00 00 00 00'
                                'FF FF 00 00'
                                '01 00 00 00')

        #pcap packet header that must preface every packet
        pcap_packet_header =   ('AA 77 9F 47'
                                '90 A2 04 00'
                                'XX XX XX XX'   #Frame Size (little endian)
                                'YY YY YY YY')  #Frame Size (little endian)

        import binascii

        all_bytes = "".join(self.bytes.split())
        print(all_bytes)
        pcap_len = self.getByteLength(all_bytes)
        print("Packet length %d"%(pcap_len))
        hex_str = "%08x"%int(pcap_len)

        reverse_hex_str = hex_str[6:] + hex_str[4:6] + hex_str[2:4] + hex_str[:2]
        pcaph = pcap_packet_header.replace('XX XX XX XX',reverse_hex_str)
        pcaph = pcaph.replace('YY YY YY YY',reverse_hex_str)

        bytestring = pcap_global_header + pcaph + all_bytes

        bytelist = bytestring.split()
        bytes = binascii.a2b_hex(''.join(bytelist))
        bitout = open(file_name, 'wb')
        bitout.write(bytes)


class IP_packet(Ethernet_packet):
    IP_HEADER = ('45'                    # IP version and header length (multiples of 4 bytes)
                 '00'
                 'XX XX'                 # Length - will be calculated and replaced later
                 '00 00'
                 '40 00 40'
                 '11'                    # Protocol (0x11 = UDP)
                 'YY YY'                 # Checksum - will be calculated and replaced later
                 'SI SI SI SI'           # Source IP (Default: 127.0.0.1)
                 'DI DI DI DI'           # Dest IP (Default: 127.0.0.1)
                 )

    def __init__(self, SRC_MAC, DST_MAC, SRC_IP, DST_IP, data_length=0, ttl=10):
        super().__init__(SRC_MAC, DST_MAC)

        ip_bytes = self.IP_HEADER.replace('SI SI SI SI', "%08x"%(SRC_IP))
        ip_bytes = ip_bytes.replace('DI DI DI DI', "%08x"%(DST_IP))
        ip_bytes = ip_bytes.replace('XX XX' , "%04x"%( data_length + self.getByteLength(self.IP_HEADER)))
        ip_bytes = ip_bytes.replace('YY YY' , "12 34") # invalid checksum

        self.bytes += ip_bytes
        print("... added IP header")
        print("\t\t" + ip_bytes)

class UDP_packet(IP_packet):
    UDP_HEADER = (
                'SP SP'                 # Source Port
                'DP DP'                 # Destination Port - will be replaced later
                'LL LL'                 # Length - will be calculated and replaced later
                '00 00')


    def __init__(self, SRC_MAC, DST_MAC, SRC_IP, DST_IP, SRC_PORT, DST_PORT, data):
        udp_length = Ethernet_packet.getByteLength(data) + Ethernet_packet.getByteLength(self.UDP_HEADER)

        super().__init__(SRC_MAC, DST_MAC, SRC_IP, DST_IP, udp_length)

        udp_bytes = self.UDP_HEADER.replace("SP SP", "%04x"%(SRC_PORT))
        udp_bytes = udp_bytes.replace("DP DP", "%04x"%(DST_PORT))
        udp_bytes = udp_bytes.replace("LL LL", "%04x"%(udp_length) )

        self.bytes += udp_bytes + data
        print("... added UDP header")
        print("\t\t" + udp_bytes)


if __name__ == "__main__":


    udp_packet = UDP_packet(
        0x12_34_56_78_90_AB,
        0x12_34_56_78_90_AC,
        192_168_1_4,
        192_168_1_5,
        0x1234,
        0x1234,
        "12 34 56 78"
        )

    udp_packet.print()

    udp_packet.write_pcap('output.pcap')
