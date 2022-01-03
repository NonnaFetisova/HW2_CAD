#include <errno.h>
#include <signal.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <map> 
#include <set>
#include <list> 
#include <algorithm>
#include "hcm.h"
#include "flat.h"
#include "hcmvcd.h"
#include "hcmsigvec.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;

bool verbose = false;
vector<hcmInstance*> global_gates;
vector<hcmNode*> global_events;
vector<hcmInstance*> global_dff; 
void or_care(hcmInstance* inst)
{
  bool result=true,node_result=true,prev_output_result =true;
	hcmInstPort* port_out;
	map<string, hcmInstPort* > inst_ports = inst->getInstPorts();  
	map<string, hcmInstPort* >::const_iterator i;
	
	for (i = inst_ports.begin(); i != inst_ports.end(); i++) 
	{
		hcmPort* i_port= i->second->getPort();
    if(i_port->getDirection()==OUT)
			port_out = i->second; 
		if (i_port->getDirection()==IN) 
		{
			 i->second->getNode()->getProp("cur_result",node_result);
			 result = result ||node_result; 
		}
	}
	hcmNode* node_out = port_out->getNode();
	node_out->getProp("cur_result",prev_output_result);

	if(prev_output_result!=result)
  { 
      global_events.push_back(node_out);
      node_out->setProp("cur_result",result);
      node_out->setProp("prev_result",prev_output_result);
  }
	else 
	    node_out->setProp("prev_result",prev_output_result);
}

void nor_care(hcmInstance* inst)
{
  bool result=true,node_result=true,prev_output_result =true;
	hcmInstPort* port_out;
	map<string, hcmInstPort* > inst_ports = inst->getInstPorts();  
	map<string, hcmInstPort* >::const_iterator i;
	
	for (i = inst_ports.begin(); i != inst_ports.end(); i++) 
	{
		hcmPort* i_port= i->second->getPort();
    if(i_port->getDirection()==OUT)
			port_out = i->second; 
		if (i_port->getDirection()==IN) 
		{
			 i->second->getNode()->getProp("cur_result",node_result);
			 result = !(result ||node_result); 
		}
	}
	hcmNode* node_out = port_out->getNode();
	node_out->getProp("cur_result",prev_output_result);

	if(prev_output_result!=result)
  { 
      global_events.push_back(node_out);
      node_out->setProp("cur_result",result);
      node_out->setProp("prev_result",prev_output_result);
  }
	else 
	    node_out->setProp("prev_result",prev_output_result);
}
void xor_care(hcmInstance* inst)
{
  bool result=true,node_result=true,prev_output_result =true;
	hcmInstPort* port_out;
	map<string, hcmInstPort* > inst_ports = inst->getInstPorts();  
	map<string, hcmInstPort* >::const_iterator i;
	
	for (i = inst_ports.begin(); i != inst_ports.end(); i++) 
	{
		hcmPort* i_port= i->second->getPort();
    if(i_port->getDirection()==OUT)
			port_out = i->second; 
		if (i_port->getDirection()==IN) 
		{
			 i->second->getNode()->getProp("cur_result",node_result);
			 result = result ^ node_result; 
		}
	}
	hcmNode* node_out = port_out->getNode();
	node_out->getProp("cur_result",prev_output_result);

	if(prev_output_result!=result)
  { 
      global_events.push_back(node_out);
      node_out->setProp("cur_result",result);
      node_out->setProp("prev_result",prev_output_result);
  }
	else 
	    node_out->setProp("prev_result",prev_output_result);
}

void xnor_care(hcmInstance* inst)
{
  bool result=true,node_result=true,prev_output_result =true;
	hcmInstPort* port_out;
	map<string, hcmInstPort* > inst_ports = inst->getInstPorts();  
	map<string, hcmInstPort* >::const_iterator i;
	
	for (i = inst_ports.begin(); i != inst_ports.end(); i++) 
	{
		hcmPort* i_port= i->second->getPort();
    if(i_port->getDirection()==OUT)
			port_out = i->second; 
		if (i_port->getDirection()==IN) 
		{
			 i->second->getNode()->getProp("cur_result",node_result);
			 result = !(result ^node_result); 
		}
	}
	hcmNode* node_out = port_out->getNode();
	node_out->getProp("cur_result",prev_output_result);

	if(prev_output_result!=result)
  { 
      global_events.push_back(node_out);
      node_out->setProp("cur_result",result);
      node_out->setProp("prev_result",prev_output_result);
  }
	else 
	    node_out->setProp("prev_result",prev_output_result);
}

void inv_care(hcmInstance* inst)
{
  bool result=true,node_result=true,prev_output_result =true;
	hcmInstPort* port_out;
	map<string, hcmInstPort* > inst_ports = inst->getInstPorts();  
	map<string, hcmInstPort* >::const_iterator i;
	
	for (i = inst_ports.begin(); i != inst_ports.end(); i++) 
	{
		hcmPort* i_port= i->second->getPort();
    if(i_port->getDirection()==OUT)
			port_out = i->second; 
		if (i_port->getDirection()==IN) 
		{
			 i->second->getNode()->getProp("cur_result",node_result);
			 result = !(node_result); 
		}
	}
	hcmNode* node_out = port_out->getNode();
	node_out->getProp("cur_result",prev_output_result);

	if(prev_output_result!=result)
  { 
      global_events.push_back(node_out);
      node_out->setProp("cur_result",result);
      node_out->setProp("prev_result",prev_output_result);
  }
	else 
	    node_out->setProp("prev_result",prev_output_result);
}

void buf_care(hcmInstance* inst)
{
  bool result=true,node_result=true,prev_output_result =true;
	hcmInstPort* port_out;
	map<string, hcmInstPort* > inst_ports = inst->getInstPorts();  
	map<string, hcmInstPort* >::const_iterator i;
	
	for (i = inst_ports.begin(); i != inst_ports.end(); i++) 
	{
		hcmPort* i_port= i->second->getPort();
    if(i_port->getDirection()==OUT)
			port_out = i->second; 
		if (i_port->getDirection()==IN) 
		{
			 i->second->getNode()->getProp("cur_result",node_result);
			 result = node_result; 
		}
	}
	hcmNode* node_out = port_out->getNode();
	node_out->getProp("cur_result",prev_output_result);

	if(prev_output_result!=result)
  { 
      global_events.push_back(node_out);
      node_out->setProp("cur_result",result);
      node_out->setProp("prev_result",prev_output_result);
  }
	else 
	    node_out->setProp("prev_result",prev_output_result);
}

void and_care(hcmInstance* inst)
{
  bool result=true,node_result=true,prev_output_result =true;
	hcmInstPort* port_out;
	map<string, hcmInstPort* > inst_ports = inst->getInstPorts();  
	map<string, hcmInstPort* >::const_iterator i;
	
	for (i = inst_ports.begin(); i != inst_ports.end(); i++) 
	{
		hcmPort* i_port= i->second->getPort();
    if(i_port->getDirection()==OUT)
			port_out = i->second; 
		if (i_port->getDirection()==IN) 
		{
			 i->second->getNode()->getProp("cur_result",node_result);
			 result = result &&node_result; 
		}
	}
	hcmNode* node_out = port_out->getNode();
	node_out->getProp("cur_result",prev_output_result);

	if(prev_output_result!=result)
  { 
      global_events.push_back(node_out);
      node_out->setProp("cur_result",result);
      node_out->setProp("prev_result",prev_output_result);
  }
	else 
	    node_out->setProp("prev_result",prev_output_result);
}

void nand_care(hcmInstance* inst)
{
  bool result=true,node_result=true,prev_output_result =true;
	hcmInstPort* port_out;
	map<string, hcmInstPort* > inst_ports = inst->getInstPorts();  
	map<string, hcmInstPort* >::const_iterator i;
	
	for (i = inst_ports.begin(); i != inst_ports.end(); i++) 
	{
		hcmPort* i_port= i->second->getPort();
    if(i_port->getDirection()==OUT)
			port_out = i->second; 
		if (i_port->getDirection()==IN) 
		{
			 i->second->getNode()->getProp("cur_result",node_result);
			 result = !(result &&node_result); 
		}
	}
	hcmNode* node_out = port_out->getNode();
	node_out->getProp("cur_result",prev_output_result);

	if(prev_output_result!=result)
  { 
      global_events.push_back(node_out);
      node_out->setProp("cur_result",result);
      node_out->setProp("prev_result",prev_output_result);
  }
	else 
	    node_out->setProp("prev_result",prev_output_result);
}

void dff_care(hcmInstance* inst)
{

  bool result=true,node_result=true,prev_output_result =true,clk=false;
	hcmInstPort* port_out;
	map<string, hcmInstPort* > inst_ports = inst->getInstPorts();  
	map<string, hcmInstPort* >::const_iterator i;
  string inst_name = inst->masterCell()->getName();
	
	for (i = inst_ports.begin(); i != inst_ports.end(); i++) 
	{
		hcmPort* i_port= i->second->getPort();
    if(i_port->getDirection()==OUT)
			port_out = i->second; 
		if (i_port->getDirection()==IN) 
		{
      if(i_port->getName() =="clk"||i_port->getName() =="CLK")
			   i->second->getNode()->getProp("cur_result",clk);
      else
			   i->second->getNode()->getProp("cur_result",node_result); 
		}
	}
	hcmNode* node_out = port_out->getNode();
	node_out->getProp("cur_result",prev_output_result);
  if(!clk)
  {
    if(prev_output_result!=result)
    { 
        global_events.push_back(node_out);
        node_out->setProp("cur_result",result);
        node_out->setProp("prev_result",prev_output_result);
    }
    else 
        node_out->setProp("prev_result",prev_output_result);
  }
}

void gate_care() 
{	
	vector<hcmInstance*>::iterator i;

	for (i =global_gates.begin(); i !=global_gates.end(); i++) 
	{
		hcmInstance* gate_inst = (*i);
		string gate_name = gate_inst->masterCell()->getName();

		if(gate_name.find("xnor")!=std::string::npos)
			xnor_care(gate_inst);

		else if(gate_name.find("xor")!=std::string::npos)
			xor_care(gate_inst);

		else if(gate_name.find("nor")!=std::string::npos)
			nor_care(gate_inst);

		else if(gate_name.find("or")!=std::string::npos)
		  or_care(gate_inst);

       else if(gate_name.find("nand")!=std::string::npos)
			and_care(gate_inst);
		
		else if(gate_name.find("and")!=std::string::npos)
			nand_care(gate_inst);

		else if(gate_name.find("buffer")!=std::string::npos)
	  	buf_care(gate_inst);

		else if(gate_name.find("inv")!=std::string::npos)
			inv_care(gate_inst);
	}
  global_gates.clear();
}

void event_care() 
{
  vector<hcmNode*>::iterator i;
	
	for(i =global_events.begin(); i != global_events.end(); i++){
    map<string, hcmInstPort* >::const_iterator i_iport;
		map<string, hcmInstPort* > iports_node = (*i)->getInstPorts(); 
		for(i_iport = iports_node.begin(); i_iport != iports_node.end(); i_iport++){
			hcmPort* i_port = i_iport->second->getPort();
			if(i_port->getDirection()==IN) {
				hcmInstance* event_gate =i_iport->second->getInst();
				global_gates.push_back(event_gate);
			}
		}
	}
	global_events.clear();	
}


int main(int argc, char **argv) {
  int argIdx = 1;
  int anyErr = 0;
  unsigned int i;
  vector<string> vlgFiles;
  bool first_simulation =true,voltage;
  if (argc < 4) {
    anyErr++;
  } else {
    if (!strcmp(argv[argIdx], "-v")) {
      argIdx++;
      verbose = true;
    }
    for (;argIdx < argc; argIdx++) {
      vlgFiles.push_back(argv[argIdx]);
    }
    
    if (vlgFiles.size() < 2) {
      cerr << "-E- At least top-level and single verilog file required for spec model" << endl;
      anyErr++;
    }
  }

  if (anyErr) {
    cerr << "Usage: " << argv[0] << "  [-v] top-cell file1.v [file2.v] ... \n";
    exit(1);
  }
 
  set< string> globalNodes;
  globalNodes.insert("VDD");
  globalNodes.insert("VSS");
  
  hcmDesign* design = new hcmDesign("design");
  string cellName = vlgFiles[0];
  for (i = 3; i < vlgFiles.size(); i++) {
    printf("-I- Parsing verilog %s ...\n", vlgFiles[i].c_str());
    if (!design->parseStructuralVerilog(vlgFiles[i].c_str())) {
      cerr << "-E- Could not parse: " << vlgFiles[i] << " aborting." << endl;
      exit(1);
    }
  }
  

  /*direct to file*/
  string fileName = cellName + string(".stat");
  ofstream fv(fileName.c_str());
  if (!fv.good()) {
    cerr << "-E- Could not open file:" << fileName << endl;
    exit(1);
  }

  hcmCell *topCell = design->getCell(cellName);
  if (!topCell) {
    printf("-E- could not find cell %s\n", cellName.c_str());
    exit(1);
  }
  
  hcmCell *flatCell = hcmFlatten(cellName + string("_flat"), topCell, globalNodes);
  cout << "-I- Top cell flattened" << endl;
  
  fv << "file name: " << fileName << endl;
  
  
	string sigsFileName = string(argv[2]),vecsFileName = string(argv[3]);
	hcmSigVec parser(sigsFileName, vecsFileName, verbose);
 	if (!parser.good())
		exit(1);
	set<string> signals_set;
	parser.getSignals(signals_set);
  	map<string, vector<bool> > signals_map;
	bool sig_value;
	
	while (!(parser.readVector())) 
	{
		 for (set<string>::iterator i= signals_set.begin(); i != signals_set.end(); i++) 
		 {
			string sig_name = (*i);
			parser.getSigValue(sig_name, sig_value);
			if(first_simulation==false) 
        signals_map.at(sig_name).push_back(sig_value);
			else{
	vector <bool> vec_i;
	vec_i.push_back(sig_value);	
        signals_map.insert(make_pair<string,vector<bool> >(sig_name, vec_i));
	}
		 }
		 first_simulation =false;
	}
 
	map<string, hcmNode*>::iterator node_i;
  map<string, hcmNode*> cell_nodes = flatCell->getNodes();
		
	for(map<string, hcmNode* >::iterator node_i =cell_nodes.begin();node_i !=cell_nodes.end(); node_i++)
	{
		hcmNode *curr_node = (*node_i).second;
     		voltage=(curr_node->getName() =="VDD")? true:false;
		curr_node->setProp("cur_result", voltage);
		curr_node->setProp("prev_result", voltage);
		if(curr_node->getName() =="VDD"||curr_node->getName()=="VSS")
      cell_nodes.erase(node_i);
	}

  map<string, hcmInstance*>::iterator instance_i;
  map<string, hcmInstance*>cell_instance =flatCell->getInstances(); 

	for(instance_i =cell_instance.begin();instance_i!=cell_instance.end();instance_i++)
	{
		hcmInstance* current_instance = (*instance_i).second;
		if(current_instance->masterCell()->getName().find("dff")!=std::string::npos)
			global_dff.push_back(current_instance);
	}

	vcdFormatter vcd(cellName +".vcd", flatCell, globalNodes); 
	if (!vcd.good()) 
	{
		printf("-E- Could not create vcdFormatter for cell: %s\n", cellName.c_str());
		exit(1);
	}
  
	vcd.changeTime(0);
  int curr_time=0,k,size_input =signals_map.begin()->second.size();
  //vector<bool> sig_vec;
  vector<hcmInstance*>::iterator i_dff;
	list<const hcmInstance*> sim_instances;
  map<string, hcmNode* >::const_iterator sim_i;
  map<string, vector<bool> >::const_iterator map_i;
	hcmNode* node_i_sim;
	bool prev_node_val,cur_sig_val;

	while(curr_time<size_input)
	{
		for(i_dff = global_dff.begin();i_dff!= global_dff.end();i_dff++)
			dff_care((*i_dff));
		for(map_i= signals_map.begin();map_i!=signals_map.end();map_i++)
		{
			string curr_name_signal =(*map_i).first;
			node_i_sim =cell_nodes[curr_name_signal];
			//sig_vec =map_i->second();
			cur_sig_val =((*map_i).second)[curr_time]; 

			node_i_sim->getProp("cur_result",prev_node_val);
			node_i_sim->setProp("prev_result",prev_node_val);	
			node_i_sim->setProp("cur_result",cur_sig_val);			
			if(prev_node_val!=cur_sig_val)
		  	global_events.push_back(node_i_sim);
		}
		while(global_events.size())
		{
			event_care(); 
			gate_care(); 
		}
		for(sim_i =cell_nodes.begin();sim_i != cell_nodes.end();sim_i++)
		{
			node_i_sim =(*sim_i).second;
			node_i_sim->getProp("cur_result",prev_node_val);
      voltage=(node_i_sim->getName() == "VSS" || node_i_sim->getName() == "VDD")? true:false;
			if(voltage)
				continue;
			hcmNodeCtx* node_Ctx = new hcmNodeCtx(sim_instances,node_i_sim);
			vcd.changeValue(node_Ctx,prev_node_val);
		}
		curr_time++; 
		vcd.changeTime(curr_time); 
	}
		 
  return(0);
}
