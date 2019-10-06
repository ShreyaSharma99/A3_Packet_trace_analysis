import java.io.*; 
import java.util.*;


class TCP_connect{
	ConnectionID id;
	int bytes;
	float t_start;
	float t_end;
	ArrayList<Packet> pack_array = new ArrayList<Packet>();

	public TCP_connect(ConnectionID id, float t_start){
		this.id = id;
		this.bytes=0;
		this.t_start = t_start;
	}

	public ConnectionID getConnectionID() 
    { 
        return id; 
    } 

    public int getBytes(){
    	return bytes;
    }

    public float getT_start() 
    { 
        return t_start; 
    } 

    public float getT_end() 
    { 
        return t_end; 
    } 

    public void setT_end(float t_end) 
    { 
        this.t_end = t_end; 
    } 

    public void setBytes(int bytes){
    	this.bytes = this.bytes + bytes;
    }

    public void inserPacket(Packet p){
    	pack_array.add(p);
    }


  
}

class Packet{
	String srceIP;
	String destIP;
	int total_bytes;
	String message;

	public Packet(String sourceIP, String destIP, int total_bytes){
		this.srcIP = sourceIP;
		this.destIP = destIP;
		this.total_bytes = total_bytes;
	}
}

class ConnectionID{
	String srcIP;
	String destIP;
	String srcPort;
	String destPort;

	public ConnectionID(String srcIP, String destIP, String srcPort, String destPort){
		this.srcIP = srcIP;
		this.destIP = destIP;
		this.srcPort = srcPort;
		this.destPort = destPort;
	}

	public String getSrcIP() 
    { 
        return srcIP; 
    } 

	public String getDestIP() 
    { 
        return destIP; 
    } 

    public String getSrcPort() 
    { 
        return srcPort; 
    } 

    public String getDestPort() 
    { 
        return destPort; 
    } 
}

public class ReadCSV(){
	String csvFile;
	ArrayList<String> serverIP_list = new ArrayList<String>();
	ArrayList<String> client_list = new ArrayList<String>();
	ArrayList<TCP_connect> TCP_list = new ArrayList<TCP_connect>();
	ArrayList<ConnectionID> live_TCP = new ArrayList<ConnectionID>();
	//constructor
	public ReadCSV(String csv){
		this.csvFile = csv;
	}

	public boolean compareID(ConnectionID id1, ConnectionID id2){

		if(id1.getSrcPort.equals(id2.getSrcPort) && id1.getDestPort.equals(id2.getDestPort) && id1.getSrcID.equals(id2.getSrcID) && id1.getDestID.equals(id2.getDestID))
			return true;
		else 
			return false;
	}

	public boolean isMember_TCP(ConnectionID id){
		for(int i=0 ; i<live_TCP.size(); i++){
			if(compareID(id, live_TCP.get(i).getConnectionID()))
				return true;
		}
		return false;
	}

	public boolean isMember(String s, ArrayList<String> list){
		for(int i=0 ; i<list.size(); i++){
			if(list.get(i).equals(s))
				return true;
		}
		return false;
	}



	public void read_data(){

		BufferedReader csvReader = new BufferedReader(new FileReader(csvFile));
		String row = csvReader.readLine();
		while ((row = csvReader.readLine()) != null) {
		    String[] data = row.split(",");
			String[] mssg = data[6].split("\\s+");
			if(!isMember(data[2],client_list)){
				client_list.add(data[2]);
			}
			if(!isMember(data[3],server_list)){
				server_list.add(data[3]);
			}
			
			if(data[4].equals("TCP")){
				if(mssg[3].equals("[SYN]")){
					ConnectionID temp_id = new ConnectionID(data[2], data[3], mssg[0], mssg[2]);
					if(!isMember_TCP(temp_id)){
						TCP_connect temp_tcp = new TCP_connect(temp_id,Float.parseFloat(data[1]));
						int byte_numb = 0;
						for(int i=0; i<mssg.size(); i++){
							if(mssg[i].substring(0,4).equals("Len=")){
								byte_numb = Integer.parseInt(mssg[i].substring(4,mssg[i].size()));
								break;
							}
						}
						temp_tcp.setBytes(byte_numb);
						TCP_list.add(temp_tcp);
						live_TCP.add(temp_id);
					}
				}
			}
			else{

			}
		}
		csvReader.close();
	}






	public static void main(String[] args) 
    { 

        ReadCSV reader = new ReadCSV("file_name"); 

        // System.out.println(tuffy.toString()); 
    } 

}