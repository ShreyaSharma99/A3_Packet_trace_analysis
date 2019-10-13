import java.io.*; 
import java.util.*;		


class TCP_connect{
	ConnectionID id;
	int bytes_sent;
	int bytes_rec;
	float t_start;
	float t_end;
	boolean ack_retransmit = false;
	boolean spur_retransmit = false;
	boolean dup_ack = false;
	boolean out_of_order = false;

	ArrayList<Packet> pack_sent = new ArrayList<Packet>();
	ArrayList<Packet> pack_rec = new ArrayList<Packet>();

	public TCP_connect(ConnectionID id, float t_start){
		this.id = id;
		this.bytes_sent=0;
		this.bytes_rec=0;
		this.t_start = t_start;
	}

	public void setAck_retransmit(boolean b){
		this.ack_retransmit = b;
	}


	public void setSpur_retransmit(boolean b){
		this.spur_retransmit = b;
	}


	public void setDup_ack(boolean b){
		this.dup_ack = b;
	}

	public boolean getDup_ack(){
		return this.dup_ack;
	}


	public void setOut_of_order(boolean b){
		this.out_of_order= b;
	}

	public ConnectionID getConnectionID() { 
        return id; 
    } 

    public ArrayList<Packet> getPack_sent(){
    	return pack_sent;
    }

    public ArrayList<Packet> getPack_rec(){
    	return pack_rec;
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
	int type;		//ack or not ack
	String seq;
	String ack;

	public Packet(String sourceIP, String destIP, float time, int total_bytes, int type){
		this.srcIP = sourceIP;
		this.destIP = destIP;
		this.total_bytes = total_bytes;
		this.time = time;
		this.seq = "";
		this.ack = "";
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

    public float getTime(){
    	return time;
    }

    public String getSeq(){
    	return seq;
    }

    public String getAck(){
    	return ack;
    }

    public int getType(){
    	return type;
    }

    public void setMssg(String m){
    	message = m;
    }

    public void setSeq(String seq){
    	this.seq = seq;
    }

    public void setAck(String ack){
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

	public String find_seq(String[] mssg){
		for(int i=0; i<mssg.length; i++){
			if(mssg[i].length()>4){
				if(mssg[i].substring(0,4).equals("Seq=")){
				return (mssg[i].substring(4,mssg[i].length()));
				}
			}
			
		}
		return "";
	}


	public String find_ack(String[] mssg){
		for(int i=0; i<mssg.length; i++){
			if(mssg[i].length()>4){
				if(mssg[i].substring(0,4).equals("Ack=")){
				return (mssg[i].substring(4,mssg[i].length()));
				}
			}
		}
		return "";
	}

	public boolean check_dup_ack(String s , ArrayList<Packet> p){
		for(int i=0 ;i<p.size(); i++){
			if(p.get(i).getAck().equals(s)){
				return true;
				// break;
			}
		}
		return false;
	}

	public boolean repeat_syn(String s , ArrayList<Packet> p){
		for(int i=0 ;i<p.size(); i++){
			if(p.get(i).getSeq().equals(s)){
				return true;
				// break;
			}
		}
		return false;
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
					if(find_seq(mssg).length()>0)
						pack.setSeq(find_seq(mssg));
					if(find_ack(mssg).length()>0)
						pack.setAck(find_ack(mssg));
					
					temp_tcp.insertPacket_sent(pack);		//to set current SYN packet 
					// TCP_list.add(temp_tcp);
					live_TCP.add(temp_tcp);
				}
				else if(mssg[3].equals("[FIN,") || mssg[3].equals("[RST]") ){		//closing a TCP connection
					Packet pack = new Packet(data[2],data[3],Float.parseFloat(data[1]),Integer.parseInt(data[5]),1);
					pack.setMssg(data[6]);
					if(find_seq(mssg).length()>0)
						pack.setSeq(find_seq(mssg));
					if(find_ack(mssg).length()>0)
						pack.setAck(find_ack(mssg));

					ConnectionID temp_id1 = new ConnectionID(data[2], data[3], mssg[0], mssg[2]);
					ConnectionID temp_id2 = new ConnectionID(data[3], data[2], mssg[2], mssg[0]);
					int ind1 = getIndex_TCP(temp_id1);
					int ind2 = getIndex_TCP(temp_id2);
					if(ind1>=0){
						live_TCP.get(ind1).setDup_ack(check_dup_ack(find_ack(mssg),live_TCP.get(ind1).getPack_sent()));
						live_TCP.get(ind1).setT_end(Float.parseFloat(data[1]));		//setting t_end for the TCP_connection
						live_TCP.get(ind1).insertPacket_sent(pack);
						
						TCP_list.add(live_TCP.get(ind1));
						live_TCP.remove(ind1);
						//do something here and also checl for above packets which cone after this command
					}
					if(ind2>=0){
						ArrayList<Packet> pack_sent = live_TCP.get(ind2).getPack_sent();
						if(repeat_syn(find_seq(mssg),live_TCP.get(ind2).getPack_rec())){
							if(check_dup_ack(Long.toString(Long.parseLong(find_seq(mssg))+1),pack_sent))
								live_TCP.get(ind2).setSpur_retransmit(true);
							else live_TCP.get(ind2).setAck_retransmit(true);
						}
						if(pack_sent.size()>1){
							if(!pack_sent.get(pack_sent.size()-1).getAck().equals(find_seq(mssg))){
								System.out.println(pack.getTime());
								live_TCP.get(ind2).setOut_of_order(true);
							}
						}
						
						live_TCP.get(ind2).setT_end(Float.parseFloat(data[1]));		//setting t_end for the TCP_connection
						live_TCP.get(ind2).insertPacket_rec(pack);
						
						TCP_list.add(live_TCP.get(ind2));
						live_TCP.remove(ind2);
					}

					// if(ind1>=0){
					// 	if(check_dup_ack(find_ack(mssg),live_TCP.get(ind1).getPack_sent()))
					// 		live_TCP.get(ind1).setDup_ack(true);

					// 	live_TCP.get(ind1).setT_end(Float.parseFloat(data[1]));		//setting t_end for the TCP_connection
					// 	live_TCP.get(ind1).insertPacket_sent(pack);

					// 	TCP_list.add(live_TCP.get(ind1));
					// 	live_TCP.remove(ind1);

					// }
					// if(ind2>=0){
					// 	ArrayList<Packet> pack_sent = live_TCP.get(ind2).getPack_sent();
					// 	if(repeat_syn(find_seq(mssg),live_TCP.get(ind1).getPack_rec())){
					// 		if(check_dup_ack(find_seq(mssg)+1,pack_sent))
					// 			live_TCP.get(ind2).setSpur_retransmit(true);
					// 		else live_TCP.get(ind2).setAck_retransmit(true);
					// 	}
					// 	if(pack_sent.get(pack_sent.size()-1).getAck()!=find_seq(mssg))
					// 		live_TCP.get(ind2).setOut_of_order(true);
					// 	live_TCP.get(ind2).setT_end(Float.parseFloat(data[1]));		//setting t_end for the TCP_connection
					// 	live_TCP.get(ind2).insertPacket_rec(pack);
						
					// 	TCP_list.add(live_TCP.get(ind2));
					// 	live_TCP.remove(ind2);
					// }

				}
				else {		//cases like SYN,ACK, or ACK or PSH,ACK
					Packet pack = new Packet(data[2],data[3],Float.parseFloat(data[1]),Integer.parseInt(data[5]),2);
					pack.setMssg(data[6]);
					if(find_seq(mssg).length()>0)
						pack.setSeq(find_seq(mssg));
					if(find_ack(mssg).length()>0)
						pack.setAck(find_ack(mssg));
					
					ConnectionID temp_id1 = new ConnectionID(data[2], data[3], mssg[0], mssg[2]);
					ConnectionID temp_id2 = new ConnectionID(data[3], data[2], mssg[2], mssg[0]);
					int ind1 = getIndex_TCP(temp_id1);
					int ind2 = getIndex_TCP(temp_id2);
					if(ind1>=0){
						// System.out.println("1 " + find_ack(mssg));
						live_TCP.get(ind1).setDup_ack(check_dup_ack(find_ack(mssg),live_TCP.get(ind1).getPack_sent()));
						live_TCP.get(ind1).insertPacket_sent(pack);
					}
					if(ind2>=0){
						// System.out.println("2 " + find_seq(mssg));

						ArrayList<Packet> pack_sent = live_TCP.get(ind2).getPack_sent();
						if(repeat_syn(find_seq(mssg),live_TCP.get(ind2).getPack_rec())){
							if(check_dup_ack(Long.toString(Long.parseLong(find_seq(mssg))+1),pack_sent))
								live_TCP.get(ind2).setSpur_retransmit(true);
							else live_TCP.get(ind2).setAck_retransmit(true);
						}
						if(pack_sent.size()>1){
							if(!pack_sent.get(pack_sent.size()-1).getAck().equals(find_seq(mssg))){
								System.out.println(pack.getTime());
								live_TCP.get(ind2).setOut_of_order(true);
							}
						}
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






	public static void main(String[] args) 
    { 

        ReadCSV reader = new ReadCSV("1.csv"); 
        reader.read_data();
        // System.out.println(tuffy.toString());
        //1420 700 -1
        //407 398 -2
        //1171 430 - 3

        ArrayList<String> serverList = reader.getServer_list();
        System.out.println("1: "+serverList.size());

        ArrayList<String> clientList = reader.getClient_list();
        System.out.println("1: "+clientList.size());

        ArrayList<TCP_connect> TCP_connections = reader.getTCP_list();
        ArrayList<TCP_connect> Live_connections = reader.getLive_TCP();

        System.out.println(TCP_connections.size());
        System.out.println(Live_connections.size());

        


    	//------Q9 (b,c,d)
    	int maxind1 = 1420;
        int maxind2 = 700;
    	int ind1 = -1;
    	int ind2 = -1;
		System.out.println("Q 9.d");
		for(int i=0; i<TCP_connections.size(); i++){
			if(TCP_connections.get(i).out_of_order && i!=maxind1){
				// System.out.println(i);
				ind1 = i;
				break;
			}	
		}
		System.out.println("ind1 " + ind1);

		for(int i=ind1+1; i<TCP_connections.size(); i++){
			if(TCP_connections.get(i).out_of_order && i!=maxind2){
				// System.out.println(i);
				ind2 = i;
				break;
			}	
		}
		System.out.println("ind2 "+ind2);

		TCP_connect tcp_1 = TCP_connections.get(0);
		TCP_connect tcp_2 = TCP_connections.get(1);

		if(ind1>=0)
			tcp_1 = TCP_connections.get(ind1);
        if(ind2 >= 0)
        	tcp_2 = TCP_connections.get(ind2);
        	
    	if(ind1>=0 && ind2>=0){
	    	ArrayList<Packet> pack_sent = tcp_1.getPack_sent();
	    	ArrayList<Packet> pack_rec = tcp_1.getPack_rec();
	        
	        String s_packet_rec = "";

	        for(int i=0; i<pack_rec.size(); i++){
	        	s_packet_rec = s_packet_rec + pack_rec.get(i).getTime() + "  " + pack_rec.get(i).getSeq() + "\n";
	        }

	        String s_packet_ack_sent = "";
	        int count = 0;
	        for(int i=0; i<pack_sent.size(); i++){
	        	if(pack_sent.get(i).getType()==2)
	        		count++;
	        		s_packet_ack_sent = s_packet_ack_sent + pack_sent.get(i).getTime() + "  " + pack_sent.get(i).getAck() + "\n";
	        }

	        try{
		      	FileWriter fw=new FileWriter("q9d_11_rec.txt");    
		        fw.write(s_packet_rec);    
		        fw.close();  
	      	}catch(Exception e){
		      	System.out.println(e);
			} 

			try{
		      	FileWriter fw=new FileWriter("q9d_11_sent.txt");    
		        fw.write(s_packet_ack_sent);    
		        fw.close();  
	      	}catch(Exception e){
		      	System.out.println(e);
			} 

			pack_sent = tcp_2.getPack_sent();
	    	pack_rec = tcp_2.getPack_rec();
	        
	    	s_packet_rec = "";
	    	System.out.println("Rec: "+pack_rec.size());
	        for(int i=0; i<pack_rec.size(); i++){
	        	s_packet_rec = s_packet_rec + pack_rec.get(i).getTime() + "  " + pack_rec.get(i).getSeq() + "\n";
	        }

	        s_packet_ack_sent = "";
	        count = 0;
	        for(int i=0; i<pack_sent.size(); i++){
	        	if(pack_sent.get(i).getType()==2)
	        		count++;
	    		s_packet_ack_sent = s_packet_ack_sent + pack_sent.get(i).getTime() + "  " + pack_sent.get(i).getAck() + "\n";
	        }

	        try{
		      	FileWriter fw=new FileWriter("q9d_12_rec.txt");    
		        fw.write(s_packet_rec);    
		        fw.close();  
	      	}catch(Exception e){
		      	System.out.println(e);
			} 

			try{
		      	FileWriter fw=new FileWriter("q9d_12_sent.txt");    
		        fw.write(s_packet_ack_sent);    
		        fw.close();  
	      	}catch(Exception e){
		      	System.out.println(e);
			} 
		}
		else{
			System.out.println("Such connections not found");
		}
//------ Q9 (a)
  //       int maxind1 = 1420;
  //       int maxind2 = 700;

  //       TCP_connect tcp_1 = TCP_connections.get(maxind1);
  //       TCP_connect tcp_2 = TCP_connections.get(maxind2);
        	
  //   	ArrayList<Packet> pack_sent = tcp_1.getPack_sent();
  //   	ArrayList<Packet> pack_rec = tcp_1.getPack_rec();
        
  //       String s_packet_rec = "";

  //       for(int i=0; i<pack_rec.size(); i++){
  //       	s_packet_rec = s_packet_rec + pack_rec.get(i).getTime() + "  " + pack_rec.get(i).getSeq() + "\n";
  //       }

  //       String s_packet_ack_sent = "";
  //       int count = 0;
  //       for(int i=0; i<pack_sent.size(); i++){
  //       	if(pack_sent.get(i).getType()==2)
  //       		count++;
  //       		s_packet_ack_sent = s_packet_ack_sent + pack_sent.get(i).getTime() + "  " + pack_sent.get(i).getAck() + "\n";
  //       }

  //       try{
	 //      	FileWriter fw=new FileWriter("q9a_11_rec.txt");    
	 //        fw.write(s_packet_rec);    
	 //        fw.close();  
  //     	}catch(Exception e){
	 //      	System.out.println(e);
		// } 

		// try{
	 //      	FileWriter fw=new FileWriter("q9a_11_sent.txt");    
	 //        fw.write(s_packet_ack_sent);    
	 //        fw.close();  
  //     	}catch(Exception e){
	 //      	System.out.println(e);
		// } 

		// pack_sent = tcp_2.getPack_sent();
  //   	pack_rec = tcp_2.getPack_rec();
        
  //   	s_packet_rec = "";
  //   	System.out.println("Rec: "+pack_rec.size());
  //       for(int i=0; i<pack_rec.size(); i++){
  //       	s_packet_rec = s_packet_rec + pack_rec.get(i).getTime() + "  " + pack_rec.get(i).getSeq() + "\n";
  //       }

  //       s_packet_ack_sent = "";
  //       count = 0;
  //       for(int i=0; i<pack_sent.size(); i++){
  //       	if(pack_sent.get(i).getType()==2)
  //       		count++;
  //   		s_packet_ack_sent = s_packet_ack_sent + pack_sent.get(i).getTime() + "  " + pack_sent.get(i).getAck() + "\n";
  //       }

  //       try{
	 //      	FileWriter fw=new FileWriter("q9a_12_rec.txt");    
	 //        fw.write(s_packet_rec);    
	 //        fw.close();  
  //     	}catch(Exception e){
	 //      	System.out.println(e);
		// } 

		// try{
	 //      	FileWriter fw=new FileWriter("q9a_12_sent.txt");    
	 //        fw.write(s_packet_ack_sent);    
	 //        fw.close();  
  //     	}catch(Exception e){
	 //      	System.out.println(e);
		// } 



//-Q6------------------------------------------------------------------
  //       ArrayList<Float> time_start = new ArrayList<Float>();

  //       for(int i=0; i<TCP_connections.size(); i++){
  //       	time_start.add(TCP_connections.get(i).getT_start());
  //       }

  //       for(int i=0; i<Live_connections.size(); i++){
  //       	time_start.add(Live_connections.get(i).getT_start());
  //       }

  //       Collections.sort(time_start);

  //       String s_6= "";

  //       for(int i=0; i<time_start.size()-1; i++){
  //       	s_6 = s_6 + Integer.toString(i+1) + "  " + Float.toString(time_start.get(i+1) - time_start.get(i)) + "\n";
  //       }
        
  //       try{
	 //      	FileWriter fw=new FileWriter("q6_1.txt");    
	 //        fw.write(s_6);    
	 //        fw.close();  
  //     	}catch(Exception e){
	 //      	System.out.println(e);
		// } 




        

//Q4-------------------------------------------------------    
//       String s_4 = "";

//       ArrayList<TCP_connect> TCPs = reader.getTCP_list();
//       for(int i=0; i<TCPs.size(); i++){
//       	s_4 = s_4 + Integer.toString(i+1) + "  " + Float.toString(TCPs.get(i).getT_end() - TCPs.get(i).getT_start()) +"\n";
//       }

//       try{
//       	FileWriter fw=new FileWriter("q4_3.txt");    
 //        fw.write(s_4);    
 //        fw.close();  
//       }catch(Exception e){
//       	System.out.println(e);
		// } 
    }

}

//start from packet 

//we update the tcp_connection in the all_TCP list only when it is closed at some FIN or RST
