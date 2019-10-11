import java.io.*; 
import java.util.*;		


class TCP_connect{
	ConnectionID id;
	int bytes_sent;
	int bytes_rec;
	float t_start;
	float t_end;

	ArrayList<Packet> pack_sent = new ArrayList<Packet>();
	ArrayList<Packet> pack_rec = new ArrayList<Packet>();

	public TCP_connect(ConnectionID id, float t_start){
		this.id = id;
		this.bytes_sent=0;
		this.bytes_rec=0;
		this.t_start = t_start;
	}

	public ConnectionID getConnectionID() 
    { 
        return id; 
    } 

    public int getBytes_sent(){
    	return bytes_sent;
    }

    public int getBytes_rec(){
    	return bytes_rec;
    }

    public float getT_start() 
    { 
        return t_start; 
    } 

    public float getT_end() 
    { 
        return t_end; 
    } 

    public void setT_start(float t_start) 
    { 
        this.t_start = t_start; 
    } 

    public void setT_end(float t_end) 
    { 
        this.t_end = t_end; 
    } 

    public void setBytes_sent(int bytes){
    	this.bytes_sent = this.bytes_sent + bytes;
    }

    public void setBytes_rec(int bytes){
    	this.bytes_rec = this.bytes_rec + bytes;
    }

    public void insertPacket_sent(Packet p){
    	for(int i=0; i<pack_sent.size(); i++){
    		if(pack_sent.get(i)==p)
    			return;
    	}
    	pack_sent.add(p);
    	this.bytes_sent = this.bytes_sent + p.getBytes();
    	return;
    }

    public void insertPacket_rec(Packet p){
    	for(int i=0; i<pack_rec.size(); i++){
    		if(pack_rec.get(i)==p)
    			return;
    	}
    	pack_rec.add(p);
    	this.bytes_rec = this.bytes_rec + p.getBytes();
    	return;
    }
  
}

class Packet{
	String srcIP;
	String destIP;
	float time;
	int total_bytes;
	String message;
	int type;
	int seq;
	int ack;

	public Packet(String sourceIP, String destIP, float time, int total_bytes, int type){
		this.srcIP = sourceIP;
		this.destIP = destIP;
		this.total_bytes = total_bytes;
		this.time = time;
		this.seq = 0;
		this.ack = 0;
		this.type = type;
	}

	public String getSrcIP() 
    { 
        return srcIP; 
    } 

	public String getDestIP() 
    { 
        return destIP; 
    } 

    public int getBytes(){
    	return total_bytes;
    }

    public String getMssg(){
    	return message;
    }

    public int getSeq(){
    	return seq;
    }

    public int getAck(){
    	return ack;
    }

    public int getType(){
    	return type;
    }

    public void setMssg(String m){
    	message = m;
    }

    public void setSeq(int seq){
    	this.seq = seq;
    }

    public void setAck(int ack){
    	this.ack = ack;
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

public class ReadCSV{
	static String csvFile;
	static ArrayList<String> server_list = new ArrayList<String>();
	static ArrayList<String> client_list = new ArrayList<String>();
	static ArrayList<TCP_connect> TCP_list = new ArrayList<TCP_connect>();			//list of all TCP connections made 
	static ArrayList<TCP_connect> live_TCP = new ArrayList<TCP_connect>();			//list of currently live TCP connections
	static int maxind1 = 0;
	static int maxind2 = 0;

	public ArrayList<String> getServer_list(){
		return server_list;
	}

	public ArrayList<String> getClient_list(){
		return client_list;
	}

	public ArrayList<TCP_connect> getTCP_list(){
		return TCP_list;
	}

	public ArrayList<TCP_connect> getLive_TCP(){
		return live_TCP;
	}




	//constructor
	public ReadCSV(String csv){
		this.csvFile = csv;
	}

	public boolean compareID(ConnectionID id1, ConnectionID id2){
		if(id1.getSrcPort().equals(id2.getSrcPort()) && id1.getDestPort().equals(id2.getDestPort()) && id1.getSrcIP().equals(id2.getSrcIP()) && id1.getDestIP().equals(id2.getDestIP()))
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

	public int getIndex_TCP(ConnectionID id){
		for(int i=0 ; i<live_TCP.size(); i++){
			if(compareID(id, live_TCP.get(i).getConnectionID()))
				return i;
		}
		return -1;
	}

	public int getIndex_TCP(String srcIP, String destIP){
		for(int i=live_TCP.size()-1 ; i>=0; i--){
			if(live_TCP.get(i).getConnectionID().getSrcIP().equals(srcIP) && live_TCP.get(i).getConnectionID().getDestIP().equals(destIP))
				return i;
		}
		return -1;
	}

	public boolean isMember(String s, ArrayList<String> list){
		for(int i=0 ; i<list.size(); i++){
			if(list.get(i).equals(s))
				return true;
		}
		return false;
	}

	public int find_seq(String[] mssg){
		for(int i=9; i<mssg.length; i++){
			if(mssg[i].length()>4){
				if(mssg[i].substring(0,4).equals("Seq=")){
				return Integer.parseInt(mssg[i].substring(4,mssg[i].length()));
				}
			}
			
		}
		return -1;
	}


	public int find_ack(String[] mssg){
		for(int i=9; i<mssg.length; i++){
			if(mssg[i].length()>4){
				if(mssg[i].substring(0,4).equals("Ack=")){
				return Integer.parseInt(mssg[i].substring(4,mssg[i].length()));
				}
			}
		}
		return -1;
	}

	public void read_data(){

		try{
		BufferedReader csvReader = new BufferedReader(new FileReader(csvFile));
		String row = csvReader.readLine();
		while ((row = csvReader.readLine()) != null) {
		    String[] data = row.split(",");
		    // System.out.println("0: "+data[0]);
		    if(data.length > 6){
		    	for(int i=7; i<data.length; i++)
		    		data[6] = data[6] + "," + data[i];
		    }
		    
		    int start=-1;
		    int end=-1;

		    for(int i=0; i<7; i++){		//removing inverted quotes
				// System.out.println(data[i]);
				start=-1;
			    end=-1;

		    	int j=0;
		    	for(j=0; j<data[i].length(); j++){
		    		if(data[i].substring(j,j+1).equals("\"")){
		    			start = j;
		    			break;
		    		}
		    	}
		    	for(j=j+1; j<data[i].length(); j++){
		    		if(data[i].substring(j,j+1).equals("\"")){
		    			end = j;
		    			break;
		    		}
		    	}

		    	if(end>start && start>=0){
		    		data[i] = data[i].substring(start+1,end);
		    		// System.out.println(data[i]);
		    	}

		    }
			String[] mssg = data[6].split("\\s+");
			// int byte_numb = Integer.parseInt(data[5]);

			// System.out.println(data[0]);
			
			if(data[4].equals("TCP")){
				if(mssg[3].equals("[SYN]")){
					if(!mssg[0].equals("21")){
						if(!isMember(data[2],client_list)){
							client_list.add(data[2]);
						}
					}
					if(mssg[2].equals("21")){
						if(!isMember(data[3],server_list)){
							server_list.add(data[3]);
						}
					}
					ConnectionID temp_id = new ConnectionID(data[2], data[3], mssg[0], mssg[2]);
					if(isMember_TCP(temp_id)){		//means SYN Ack was lost and so again initailised the same connection
						int i = getIndex_TCP(temp_id);
						live_TCP.remove(i);		//remove from live_TCP list

					}
					TCP_connect temp_tcp = new TCP_connect(temp_id,Float.parseFloat(data[1]));
					Packet pack = new Packet(data[2],data[3],Float.parseFloat(data[1]),Integer.parseInt(data[5]),1);
					pack.setMssg(data[6]);
					if(find_seq(mssg)>0)
						pack.setSeq(find_seq(mssg));
					if(find_ack(mssg)>0)
						pack.setAck(find_ack(mssg));
					
					temp_tcp.insertPacket_sent(pack);		//to set current SYN packet 
					// TCP_list.add(temp_tcp);
					live_TCP.add(temp_tcp);
				}
				else if(mssg[3].equals("[FIN,") || mssg[3].equals("[RST]") ){		//closing a TCP connection
					Packet pack = new Packet(data[2],data[3],Float.parseFloat(data[1]),Integer.parseInt(data[5]),1);
					pack.setMssg(data[6]);
					if(find_seq(mssg)>0)
						pack.setSeq(find_seq(mssg));
					if(find_ack(mssg)>0)
						pack.setAck(find_ack(mssg));

					ConnectionID temp_id1 = new ConnectionID(data[2], data[3], mssg[0], mssg[2]);
					ConnectionID temp_id2 = new ConnectionID(data[3], data[2], mssg[2], mssg[0]);
					int ind1 = getIndex_TCP(temp_id1);
					int ind2 = getIndex_TCP(temp_id2);
					if(ind1>=0){
						live_TCP.get(ind1).setT_end(Float.parseFloat(data[1]));		//setting t_end for the TCP_connection
						live_TCP.get(ind1).insertPacket_sent(pack);
						
						TCP_list.add(live_TCP.get(ind1));
						live_TCP.remove(ind1);
						//do something here and also checl for above packets which cone after this command
					}
					if(ind2>=0){
						live_TCP.get(ind2).setT_end(Float.parseFloat(data[1]));		//setting t_end for the TCP_connection
						live_TCP.get(ind2).insertPacket_rec(pack);
						
						TCP_list.add(live_TCP.get(ind2));
						live_TCP.remove(ind2);
					}

				}
				else {		//cases like SYN,ACK, or ACK or PSH,ACK
					Packet pack = new Packet(data[2],data[3],Float.parseFloat(data[1]),Integer.parseInt(data[5]),1);
					pack.setMssg(data[6]);
					if(find_seq(mssg)>0)
						pack.setSeq(find_seq(mssg));
					if(find_ack(mssg)>0)
						pack.setAck(find_ack(mssg));
					
					ConnectionID temp_id1 = new ConnectionID(data[2], data[3], mssg[0], mssg[2]);
					ConnectionID temp_id2 = new ConnectionID(data[3], data[2], mssg[2], mssg[0]);
					int ind1 = getIndex_TCP(temp_id1);
					int ind2 = getIndex_TCP(temp_id2);
					if(ind1>=0){
						live_TCP.get(ind1).insertPacket_sent(pack);
					}
					if(ind2>=0){
						live_TCP.get(ind2).insertPacket_rec(pack);
					}
				}
			}
			//FTP
			// else{
			// 	Packet pack = new Packet(data[2],data[3],Float.parseFloat(data[1]),Integer.parseInt(data[5]),2);
			// 	pack.setMssg(data[6]);
				
			// 	int seq = find_seq(mssg);
			// 	int ack = find_ack(mssg);
			// 	// System.out.println("here "+seq);
			// 	if((seq)>0)
			// 		pack.setSeq(seq);
			// 	if(ack>0)
			// 		pack.setAck(ack);
			// 	// System.out.println("here2");	
			// 	int ind1 = getIndex_TCP(data[2],data[3]);
			// 	int ind2 = getIndex_TCP(data[3],data[2]);
			// 	if(ind1>=0){
			// 		live_TCP.get(ind1).insertPacket_sent(pack);
			// 	}
			// 	if(ind2>0){
			// 		live_TCP.get(ind2).insertPacket_rec(pack);
			// 	}
			// 	// System.out.println("here3");
			// }
			// // System.out.println("done");
		}
		csvReader.close();


		}catch(Exception e){
			System.out.println(e);
		}
	}

	public static void main(String[] args) throws IOException
    { 
    	String filenum = "3";
    	String csvname = filenum + ".csv";
        ReadCSV reader = new ReadCSV(csvname); 
        reader.read_data();
        // System.out.println(tuffy.toString()); 


        //Question 3
        int[] num_connections = new int[24];
        for(int i=0; i<TCP_list.size();i++){
        	int temp = (int)(TCP_list.get(i).getT_start()/3600);
        	num_connections[temp]++;
        }
        for(int i=0; i<live_TCP.size();i++){
        	int temp = (int)(live_TCP.get(i).getT_start()/3600);
        	num_connections[temp]++;
        }
        String answer3 = "";
        for(int i=0; i<24;i++){
        	if(i!=23)
        		answer3 += Integer.toString(i) + " " +  Integer.toString(i+1) + " " + Integer.toString(num_connections[i]) + "\n";
        	else
        		answer3 += Integer.toString(i) + " " +  Integer.toString(i+1) + " " + Integer.toString(num_connections[i]);
        }
        String file = "q3_" + filenum + ".txt";
        FileWriter fw = new FileWriter(file);
        fw.write(answer3);
        fw.close();

        //Question 5
        String answer5 = "";
        int max1= TCP_list.get(0).pack_sent.size() + TCP_list.get(0).pack_rec.size();
        int max2 = TCP_list.get(0).pack_sent.size() + TCP_list.get(0).pack_rec.size();
        for(int i=0; i<TCP_list.size();i++){
        	//System.out.println(TCP_list.get(i).pack_rec.size());
        	int tmp = TCP_list.get(i).pack_sent.size() + TCP_list.get(i).pack_rec.size();
        	if(tmp > max1){
        		max2 = max1;
        		max1 = tmp;
        		maxind2 = maxind1;
        		maxind1 = i;
        	}
        	ConnectionID temp = TCP_list.get(i).getConnectionID();
        	answer5 += temp.getSrcIP() + " " + temp.getDestIP()+ " " + temp.getSrcPort() + " " + temp.getDestPort();
        	if(i != TCP_list.size()-1)
        		answer5 += " " + Integer.toString(TCP_list.get(i).getBytes_sent() - (20*TCP_list.get(i).pack_sent.size())) + " " + Integer.toString(TCP_list.get(i).getBytes_rec()- (20*TCP_list.get(i).pack_rec.size())) +"\n";
        	else
        		answer5 += " " + Integer.toString(TCP_list.get(i).getBytes_sent() - (20*TCP_list.get(i).pack_sent.size())) + " " + Integer.toString(TCP_list.get(i).getBytes_rec()- (20*TCP_list.get(i).pack_rec.size()));
        }
		file = "q5_" + filenum + ".txt";
        FileWriter fw1 = new FileWriter(file);
        fw1.write(answer5);
        fw1.close();       
        System.out.println(maxind1 + " " + maxind2);

        //Question 7
        String answer7 = "";
        for(int i=0; i<TCP_list.size();i++){
        	ArrayList<Packet> temp_list = TCP_list.get(i).pack_sent;
        	for(int j=1; j<temp_list.size();j++)
        		answer7 += Float.toString(temp_list.get(j).time - temp_list.get(j-1).time) + "\n"; 
        }
		file = "q7_" + filenum + ".txt";
        FileWriter fw2 = new FileWriter(file);
        fw2.write(answer7);
        fw2.close();        

        //Question 8
        String answer8 = "";
		file = "q8_" + filenum + "_bytes_sent.txt";
        FileWriter fw3 = new FileWriter(file);
        for(int i=0; i<TCP_list.size();i++){
        	ArrayList<Packet> temp_list = TCP_list.get(i).pack_sent;
        	for(int j=0; j<temp_list.size();j++)
        		answer8 += Integer.toString(temp_list.get(j).total_bytes) + "\n"; 
        }
        fw3.write(answer8);
        fw3.close();
        answer8 = "";
        for(int i=0; i<TCP_list.size();i++){
        	ArrayList<Packet> temp_list = TCP_list.get(i).pack_rec;
        	for(int j=0; j<temp_list.size();j++){
        		if( i != TCP_list.size()-1 && j != temp_list.size()-1)
        			answer8 += Integer.toString(temp_list.get(j).total_bytes) + "\n"; 
        		else
        			answer8 += Integer.toString(temp_list.get(j).total_bytes); 
        	}
        }
        file = "q8_" + filenum + "_bytes_rec.txt";
        FileWriter fw4 = new FileWriter(file);
        fw4.write(answer8);
        fw4.close();


    } 

}

//start from packet 

//we update the tcp_connection in the all_TCP list only when it is closed at some FIN or RST

//For q7 we need to maintain all the packets sent to server may those be the ones whose reply is lost in the process. 
