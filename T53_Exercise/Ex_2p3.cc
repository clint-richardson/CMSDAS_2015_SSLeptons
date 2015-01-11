#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "ObjectID.C"

const double M_EL = 0.000510998928; //Mass of electron in GeV
const double M_MU = 0.1056583715;   //Mass of muon in GeV
const double M_Z  = 91.1876;        //Mass of Z boson
const double dM   = 15;             //Size of window around Z


void Ex_2p3(){


  ///PLACE TO ADD IN NUMBERS FOR PROMPT AND FAKE RATE
  float f_e  = 0.1;
  float p_e  = 1.0;
  float f_mu = 0.1;
  float p_mu = 1.0;

  //variables we will use later based on that:
  float eps_e  = (f_e) / (1 - f_e);
  float eta_e  = (1 - p_e) / (p_e);
  float eps_mu = (f_mu) / (1 - f_mu);
  float eta_mu = (1 - p_mu) / (p_mu);

  //total numver of events
  float Npf_data;
  float Nfp_data;
  float Nff_data;
  float Nfp_mc;
  float Nff_mc;

  //load the 'data' and mc
  TFile* fdata = new TFile("ljmet_tree_TT1_temp.root");
  //TFile* fmc   = new TFile("ljmet_tree_TT2.root");

  TTree* tdata = fdata->Get("ljmet");
  //  TTree* tmc   = fmc->Get("ljmet");


  // LOAD THINGS FOR DATA
  //histograms
  TH1F* ttHThist_data = new TH1F("ttHThist_data","H_{T} for events with two tight leptons",200,1000.,2000);
  TH1F* tlHThist_data = new TH1F("tlHThist_data","H_{T} for events with one tight and one loose leptons",200,1000.,2000);
  TH1F* llHThist_data = new TH1F("llHThist_data","H_{T} for events with two loose leptons",200,1000.,2000);

  //count of events with tight/loose leptons
  int Ntt_data = 0;
  int Ntl_data = 0;
  int Nll_data = 0;
  //count events for each cut
  int Nmasscut_data=0;
  int Nmetcut_data=0;
  int NHTcut_data=0;
  int Ndilepcut_data=0;
  int NSSdilepcut_data=0;
  int NJetscut_data=0;
  int NJetpt2cut_data=0;
  int NJetpt1cut_data=0;

 

  //initialize variables
  int nEntriesdata = tdata->GetEntries();
  //  int nEntriesmc   = tmc->GetEntries();

  //electron charges
  vector<int> *elCharge_data = 0;
  //invariant mass
  //vector<double> *diElMass_data = 0;
  //kinematic variables
  vector<double> *elpts_data = 0;
  vector<double> *elEtas_data =0;
  //variables for tracking cuts
  vector<double> *elDeta_data =0;
  vector<double> *elDphi_data =0;
  //variables for primary vtx cuts
  vector<double> *elDZs_data = 0;
  vector<double> *elD0s_data = 0;
  // H over E
  vector<double> *elHoverEs_data = 0;
  //missing hits
  vector<int> *elMHits_data = 0;
  //ooemoop
  vector<double> *elOoEmooPs_data = 0;
  //charged isolation
  vector<double> *elRelIsos_data = 0;
  //sigmaIetaIeta
  vector<double>* elSigmaIetaIetas_data = 0;
  //jets
  vector<double>* jetPts_data = 0;
  //met
  double met_data = 0;

  //set branch addresses
  //tdata->SetBranchAddress("diElMass_DileptonCalc", &diElMass_data);
  tdata->SetBranchAddress("elPt_DileptonCalc", &elpts_data);
  tdata->SetBranchAddress("elEta_DileptonCalc", &elEtas_data);
  tdata->SetBranchAddress("elDeta_DileptonCalc", &elDeta_data);
  tdata->SetBranchAddress("elDphi_DileptonCalc", &elDphi_data);
  tdata->SetBranchAddress("elDZ_DileptonCalc", &elDZs_data);
  tdata->SetBranchAddress("elD0_DileptonCalc", &elD0s_data);
  tdata->SetBranchAddress("elHoE_DileptonCalc",&elHoverEs_data);
  tdata->SetBranchAddress("elMHits_DileptonCalc",&elMHits_data);
  tdata->SetBranchAddress("elOoemoop_DileptonCalc",&elOoEmooPs_data);
  tdata->SetBranchAddress("elRelIso_DileptonCalc",&elRelIsos_data);
  tdata->SetBranchAddress("elSihih_DileptonCalc",&elSigmaIetaIetas_data);
  tdata->SetBranchAddress("corr_met_DileptonCalc",&met_data);
  tdata->SetBranchAddress("AK5JetPt_DileptonCalc",&jetPts_data);
  tdata->SetBranchAddress("elCharge_DileptonCalc",&elCharge_data);
  // tdata->SetBranchAddress("elCharge2_DileptonCalc",&elCharge2_data);
  
  //bools for data ids
  //tracking cuts
  bool elDEtaLoosecut1data = false; // fabs() <0.181;
  bool elDEtaLoosecut2data = false; // fabs() <0.181;
  bool elDEtaTightcut1data = false; // fabs() < 0.0091;
  bool elDEtaTightcut2data = false; //fabs() < 0.0091;
  bool elDPhiLoosecut1data = false; // fabs() <0.181;
  bool elDPhiLoosecut2data = false; // fabs() <0.181;
  bool elDPhiTightcut1data = false; // fabs() < 0.0091;
  bool elDPhiTightcut2data = false; //fabs() < 0.0091;
  //shower cuts
  bool elSigmaIetaIetaLoosecut1data = false;
  bool elSigmaIetaIetaLoosecut2data = false;
  bool elSigmaIetaIetaTightcut1data = false;
  bool elSigmaIetaIetaTightcut2data = false;
  //H/E cuts
  bool elHELoosecut1data = false;
  bool elHELoosecut2data = false;
  bool elHETightcut1data = false;
  bool elHETightcut2data = false;
  //primary vertex cuts
  bool elD0Loosecut1data = false;
  bool elD0Loosecut2data = false;
  bool elD0Tightcut1data = false;
  bool elD0Tightcut2data = false;
  bool elDZLoosecut1data = false;
  bool elDZLoosecut2data = false;
  bool elDZTightcut1data = false;
  bool elDZTightcut2data = false;
  //ooEooP cuts
  bool elooEmooPLoosecut1data = false;
  bool elooEmooPLoosecut2data = false;
  bool elooEmooPTightcut1data = false;
  bool elooEmooPTightcut2data = false;
  //isolation cuts
  bool elPFIsoLoosecut1data = false;
  bool elPFIsoLoosecut2data = false;
  bool elPFIsoTightcut1data = false;
  bool elPFIsoTightcut2data = false;
  //conversion rejection cuts
  bool elVtxFitProbLoosecut1data = false;
  bool elVtxFitProbLoosecut2data = false;
  bool elVtxFitProbTightcut1data = false;
  bool elVtxFitProbTightcut2data = false;
  bool elMissingHitsLoosecut1data = false;
  bool elMissingHitsLoosecut2data = false;
  bool elMissingHitsTightcut1data = false;
  bool elMissingHitsTightcut2data = false;

  //booleans for final ids
  bool elLoose1 = false; //and of all loose cuts
  bool elTight1 = false; //and of all tight cuts
  bool elLoose2 = false; //and of all loose cuts
  bool elTight2 = false; //and of all tight cuts

  //LOAD THINGS FOR MC

  vector<double> *diElMass_mc = 0;
  //kinematic variables
  vector<double> *elpts_mc = 0;
  vector<double> *elEtas_mc =0;
  //variables for tracking cuts
  vector<double> *elDeta_mc =0;
  vector<double> *elDphi_mc =0;
  //variables for primary vtx cuts
  vector<double> *elDZs_mc = 0;
  vector<double> *elD0s_mc = 0;
  // H over E
  vector<double> *elHoverEs_mc = 0;
  //missing hits
  vector<int> *elMHits_mc = 0;
  //ooemoop
  vector<double> *elOoEmooPs_mc = 0;
  //charged isolation
  vector<double> *elRelIsos_mc = 0;
  //sigmaIetaIeta
  vector<double>* elSigmaIetaIetas_mc = 0;

  //set branch addresses
  //tmc->SetBranchAddress("diElMass_DileptonCalc", &diElMass_mc);
  //tmc->SetBranchAddress("elPt_DileptonCalc", &elpts_mc);
  //tmc->SetBranchAddress("elEta_DileptonCalc", &elEtas_mc);
  //tmc->SetBranchAddress("elDeta_DileptonCalc", &elDeta_mc);
  //tmc->SetBranchAddress("elDphi_DileptonCalc", &elDphi_mc);
  //tmc->SetBranchAddress("elDZ_DileptonCalc", &elDZs_mc);
  //tmc->SetBranchAddress("elD0_DileptonCalc", &elD0s_mc);
  //tmc->SetBranchAddress("elHoE_DileptonCalc",&elHoverEs_mc);
  //tmc->SetBranchAddress("elMHits_DileptonCalc",&elMHits_mc);
  //tmc->SetBranchAddress("elOoemoop_DileptonCalc",&elOoEmooPs_mc);
  //tmc->SetBranchAddress("elRelIso_DileptonCalc",&elRelIsos_mc);
  //tmc->SetBranchAddress("elSihih_DileptonCalc",&elSigmaIetaIetas_mc);


  /*
    DATA PROCEDURE: WE JUST DO THE ANALYSIS BUT GET NUMBERS FOR HOW MANY LOOSE
    AND HOW MANY TIGHT LEPTONS THERE ARE
   */

  //event loop - plots to produce: HT distribution for all cuts except 
  for(int ient = 0; ient < nEntriesdata; ient++){

    tdata->GetEntry(ient);
    /*
      Since the event cuts not having to do with leptons do not require loops, while those for leptons do, let's apply all the non-loop needing
      cuts first in order to speed things up.
     */
    //check met req
    if( met_data < 100) continue;
    Nmetcut_data +=1;

    //check lep1pt req
    if(elpts_data->at(0) < 80) continue;

    //check subleading lep pt req
    if(elpts_data->at(1) < 30) continue;

   //require more than one jet
    if(jetPts_data->size() < 2) continue;
    NJetscut_data +=1;

    //check for high pt jet
    if(jetPts_data->at(0) < 150) continue;
    NJetpt1cut_data+=1;

    //check for second jet
    if(jetPts_data->at(1) < 50) continue;
    NJetpt2cut_data+=1;

    //check HT req
    float HT = 0;
    for(int ijet = 0; ijet<jetPts_data->size(); ijet++){
      HT+= fabs(jetPts_data->at(ijet));
    }

    if( HT < 800) continue;
    NHTcut_data+=1;

    //Put electrons back together into coherent objects and add to lepton vector
    vector <Lepton*> vLep;
    for (unsigned int uiEl = 0; uiEl < elPts->size(); uiEl++){
      Electron* el = new Electron;
      
      el->pt                = elPts->at(uiEl);
      el->eta               = elEtas->at(uiEl);
      el->phi               = elPhis->at(uiEl);
      el->charge            = elCharge->at(uiEl);
      el->dEta              = elDeta->at(uiEl);
      el->dPhi              = elDphi->at(uiEl);
      el->dZ                = elDZs->at(uiEl);
      el->d0                = elD0s->at(uiEl);
      el->hOverE            = elHoverEs->at(uiEl);
      el->mHits             = elMHits->at(uiEl);
      el->ooEmooP           = elOoEmooPs->at(uiEl);
      el->relIso            = elRelIsos->at(uiEl);
      el->sigmaIetaIeta     = elSigmaIetaIetas->at(uiEl);
      el->chargeConsistency = elChargeConsistency->at(uiEl);
      el->isEl              = true;
      el->isMu              = false;
      vLep.push_back(el);
    }

    //add muons to lepton vector
    for (unsigned int uiMu = 0; uiMu <muPts->size(); uiMu++){
      Muon* mu = new Muon;

      mu->pt      = muPts->at(uiMu);
      mu->eta     = muEtas->at(iuMu);
      mu->phi     = muPhis->at(uiMu);
      mu->isLoose = muLooseIDs->at(uiMu);
      mu->isTight = muTightIDs->at(uiMu);
      mu->isEl    = false;
      mu->isMu    = true;

      vLept.push_back(mu);
    }

    for(unsigned int ui = 0; ui<VLep.size();ui++){
      //loop over all electrons and make sure none come from Z
      bool foundPair = false; 
      for(unsigned int uj = ui + 1; uj < VLep.size(); uj++){
	//	if (!VLep.at(uj)->tight()) continue;

	TLorentzVector v1, v2;
	if(VLep.at(ui)->isEl){
	  v1.SetPtEtaPhiM(VLep.at(ui)->pt, VLep.at(ui)->eta, VLep.at(ui)->phi, M_EL);
	}
	else{
	  v1.SetPtEtaPhiM(VLep.at(ui)->pt, VLep.at(ui)->eta, VLep.at(ui)->phi, M_MU);
	}
	if(VLep.at(uj)->isEl){
	  v2.SetPtEtaPhiM(VLep.at(uj)->pt, VLep.at(uj)->eta, VLep.at(uj)->phi, M_EL);
	}
	else{
	  v2.SetPtEtaPhiM(VLep.at(uj)->pt, VLep.at(uj)->eta, VLep.at(uj)->phi, M_MU);
	}

	double mass = (v1+v2).M();
	if (mass > M_Z - dM && mass < M_Z + dM){
	  foundPair = true;
	}

      }//End loop over second lepton
      if (foundPair) break;
    }//End loop over first lepton

    //now skip event if there is a pair which comes from z boson
    if(foundPair) continue;

    //now check for same sign:
    bool samesign = false;   
    //vector to hold the leptons that are same sign
    vector<Lepton*> vSSLep;

    for(unsigned int uiLep = 0; uiLep<VLep.size(); uiLep++){
      //make sure lepton at least passes loose requirement
      if(! ( VLep.at(uiLep)->tight() || vLep.at(uiLep)->loose())) continue;
      //get charge
      int charge1 = VLep.at(uiLep)->charge;
      for(unsigned int ujLep = uiLep+1; ujLep < VLep.size(); ujLep++){
	//make sure second lepton is at least loose
	if(! ( VLep.at(ujLep)->tight() || vLep.at(ujLep)->loose())) continue;
	if( charge1 == VLep.at(ujLep)->charge) samesign = true;
      }
      if(samesign){
	vSSLep.push_back(VLep.at(uiLep));
	vSSLep.push_back(VLep.at(ujLep));
      }
      if(samesign) break;
    }

    //skip event without same sign leptons
    if(!samesign) continue;

    //now get lepton id information
    if(vSSLep.at(0)->tight()){
      if(vSSLep.at(1)->tight()) Ntt_data +=1;
      else{
	if(vSSLep.at(0).isEl){
	  if(vSSLep.at(1).isEl){
	    //???
	    }
	  else if(vSSLep.at(1).isMu){
	    Ntl_data+=1;
      }
    /*

    //check dilepton cut

    if(elpts_data->size() < 2) continue;
    Ndilepcut_data+=1;
    //check samesign lepton req
    if(elCharge1_data->at(0) != elCharge2_data->at(0)) continue;
    NSSdilepcut_data+=1;
    //z mass veto
    if((diElMass_data->size()>0)){

      if( (diElMass_data->at(0) > 76) && (diElMass_data->at(0) <116) )	continue;
    }

    Nmasscut_data +=1;

    //std::cout<<"passed all analysis cuts"<<std::endl;
    
    //now that we have made the objects pass all the analysis cuts other than ID we can see how many events there are with 0, 1, and 2 tight leptons
    
    //loop for id
    //need to separate between barrel and endcap
    //first lepton
    if(fabs(elEtas_data->at(0)) <= 1.479){
      //check delta Eta between track and supercluster
      if(fabs(elDeta_data->at(0)) < 0.0181) elDEtaLoosecut1data = true;
      if(fabs(elDeta_data->at(0)) < 0.0091) elDEtaTightcut1data = true;
      //check delta phi between track and supercluster
      if(fabs(elDphi_data->at(0)) < 0.0936) elDPhiLoosecut1data = true;
      if(fabs(elDphi_data->at(0)) < 0.031) elDPhiTightcut1data = true;
      //check sigmaIetaIeta
      if(elSigmaIetaIetas_data->at(0) < 0.0123) elSigmaIetaIetaLoosecut1data = true;
      if(elSigmaIetaIetas_data->at(0) < 0.0106) elSigmaIetaIetaTightcut1data = true;
      //check H over E
      if(elHoverEs_data->at(0) < 0.141) elHELoosecut1data = true;
      if(elHoverEs_data->at(0) < 0.0532) elHETightcut1data = true;
      //check vertex cuts
      if(fabs(elD0s_data->at(0)) < 0.0166) elD0Loosecut1data = true;
      if(fabs(elD0s_data->at(0)) < 0.0126) elD0Tightcut1data = true;
      if(fabs(elDZs_data->at(0)) < 0.54342) elDZLoosecut1data = true;
      if(fabs(elDZs_data->at(0)) < 0.0116) elDZTightcut1data = true;
      //ooEmooP cuts
      if(fabs(elOoEmooPs_data->at(0)) < 0.1353) elooEmooPLoosecut1data = true;
      if(fabs(elOoEmooPs_data->at(0)) < 0.0609) elooEmooPTightcut1data = true;
      //pfIsoCuts
      if(elRelIsos_data->at(0) < 0.24) elPFIsoLoosecut1data = true;
      if(elRelIsos_data->at(0) < 0.1649) elPFIsoTightcut1data = true;
      //missing hits cuts
      if(elMHits_data->at(0) <= 1) elMissingHitsLoosecut1data = true;
      if(elMHits_data->at(0) <= 1) elMissingHitsTightcut1data = true;
    }
    else{
      //check delta Eta between track and supercluster
      if(fabs(elDeta_data->at(0)) < 0.0124) elDEtaLoosecut1data = true;
      if(fabs(elDeta_data->at(0)) < 0.0106) elDEtaTightcut1data = true;
      //check delta phi between track and supercluster
      if(fabs(elDphi_data->at(0)) < 0.0642) elDPhiLoosecut1data = true;
      if(fabs(elDphi_data->at(0)) < 0.0359) elDPhiTightcut1data = true;
      //check sigmaIetaIeta
      if(elSigmaIetaIetas_data->at(0) < 0.035) elSigmaIetaIetaLoosecut1data = true;
      if(elSigmaIetaIetas_data->at(0) < 0.0305) elSigmaIetaIetaTightcut1data = true;
      //check H over E
      if(elHoverEs_data->at(0) < 0.1115) elHELoosecut1data = true;
      if(elHoverEs_data->at(0) < 0.0835) elHETightcut1data = true;
      //check vertex cuts
      if(fabs(elD0s_data->at(0)) < 0.098) elD0Loosecut1data = true;
      if(fabs(elD0s_data->at(0)) < 0.0163) elD0Tightcut1data = true;
      if(fabs(elDZs_data->at(0)) < 0.9187) elDZLoosecut1data = true;
      if(fabs(elDZs_data->at(0)) < 0.5999) elDZTightcut1data = true;
      //ooEmooP cuts
      if(fabs(elOoEmooPs_data->at(0)) < 0.1443) elooEmooPLoosecut1data = true;
      if(fabs(elOoEmooPs_data->at(0)) < 0.1126) elooEmooPTightcut1data = true;
      //pfIsoCuts
      if(elRelIsos_data->at(0) < 0.3529) elPFIsoLoosecut1data = true;
      if(elRelIsos_data->at(0) < 0.2075) elPFIsoTightcut1data = true;
      //missing hits cuts
      if(elMHits_data->at(0) <= 1) elMissingHitsLoosecut1data = true;
      if(elMHits_data->at(0) <= 1) elMissingHitsTightcut1data = true;
    }
    
    //now for second lepton
    if(fabs(elEtas_data->at(1))<=1.479){
      //check delta Eta between track and supercluster	
      if(fabs(elDeta_data->at(1)) < 0.0181){
	elDEtaLoosecut2data = true; 
	//std::cout<<"passed deta2 loose"<<stdl::endl;
      }
      if(fabs(elDeta_data->at(1)) < 0.0091){
	elDEtaTightcut2data = true;
	//std::cout<<"passed detae tight"<<stdl::endl;
      }
      //check delta phi between track and supercluster
      if(fabs(elDphi_data->at(1)) < 0.0936){
	elDPhiLoosecut2data = true;
	//std::cout<<"passed dphi2 loose"<<stdl::endl;
      }
      if(fabs(elDphi_data->at(1)) < 0.031){
	elDPhiTightcut2data = true;
	//std::cout<<"passed dphi2 tight"<<stdl::endl;
      }
      //check sigmaIetaIeta
      if(elSigmaIetaIetas_data->at(1) < 0.0123){
	elSigmaIetaIetaLoosecut2data = true;
	//std::cout<<"passed dsigIeIe 2 loose"<<stdl::endl;
      }
      if(elSigmaIetaIetas_data->at(1) < 0.0106){
	elSigmaIetaIetaTightcut2data = true;
	//std::cout<<"passed dsidIeIe2 tight"<<stdl::endl;
      }
      //check H over E
      if(elHoverEs_data->at(1) < 0.141){
	elHELoosecut2data = true;
	//std::cout<<"passed HE 2 loose"<<stdl::endl;
      }
      if(elHoverEs_data->at(1) < 0.0532){
	elHETightcut2data = true;
	//std::cout<<"passed HE 2 tight"<<stdl::endl;
      }
      //check vertex cuts
      if(fabs(elD0s_data->at(1)) < 0.0166){
	elD0Loosecut2data = true;
	//std::cout<<"passed d0 2 loose"<<stdl::endl;
      }
      if(fabs(elD0s_data->at(1)) < 0.0126){
	elD0Tightcut2data = true;
	//std::cout<<"passed d0 2 tight"<<stdl::endl;
      }
      if(fabs(elDZs_data->at(1)) < 0.54342){
	elDZLoosecut2data = true;
	//std::cout<<"passed dZ loose"<<stdl::endl;
      }
      if(fabs(elDZs_data->at(1)) < 0.0116){
	elDZTightcut2data = true;
	//std::cout<<"passed dZ 2 tight"<<stdl::endl;
      }
      //ooEmooP cuts
      if(fabs(elOoEmooPs_data->at(1)) < 0.1353){
	elooEmooPLoosecut2data = true;
	//std::cout<<"passed ooEmooP 2 loose"<<stdl::endl;
      }
      if(fabs(elOoEmooPs_data->at(1)) < 0.0609){
	elooEmooPTightcut2data = true;
	//std::cout<<"passed ooEmooP 2 tight"<<stdl::endl;
      }
      //pfIsoCuts
      if(elRelIsos_data->at(1) < 0.24){
	elPFIsoLoosecut2data = true;
	//std::cout<<"passed pfiso 2 loose"<<stdl::endl;
      }
      if(elRelIsos_data->at(1) < 0.1649){
	elPFIsoTightcut2data = true;
	//std::cout<<"passed pfiso 2 tight"<<stdl::endl;
      }
      //missing hits cuts
      if(elMHits_data->at(1) <= 1){
	elMissingHitsLoosecut2data = true;
	//std::cout<<"passed missing hits 2 loose"<<stdl::endl;
      }
      if(elMHits_data->at(1) <= 1){
	elMissingHitsTightcut2data = true;
	//std::cout<<"passed missing hist 2 tight"<<stdl::endl;
      }
    }
    
    else{
      //check delta Eta between track and supercluster	
      if(fabs(elDeta_data->at(1)) < 0.0124) elDEtaLoosecut2data = true;

      if(fabs(elDeta_data->at(1)) < 0.0106) elDEtaTightcut2data = true;

      //check delta phi between track and supercluster
      if(fabs(elDphi_data->at(1)) < 0.0642) elDPhiLoosecut2data = true;

      if(fabs(elDphi_data->at(1)) < 0.0359) elDPhiTightcut2data = true;

      //check sigmaIetaIeta
      if(elSigmaIetaIetas_data->at(1) < 0.035) elSigmaIetaIetaLoosecut2data = true;

      if(elSigmaIetaIetas_data->at(1) < 0.0305) elSigmaIetaIetaTightcut2data = true;

      //check H over E
      if(elHoverEs_data->at(1) < 0.1115) elHELoosecut2data = true;

      if(elHoverEs_data->at(1) < 0.0835) elHETightcut2data = true;

      //check vertex cuts
      if(fabs(elD0s_data->at(1)) < 0.098) elD0Loosecut2data = true;

      if(fabs(elD0s_data->at(1)) < 0.0163) elD0Tightcut2data = true;

      if(fabs(elDZs_data->at(1)) < 0.9187) elDZLoosecut2data = true;

      if(fabs(elDZs_data->at(1)) < 0.5999) elDZTightcut2data = true;

      //ooEmooP cuts
      if(fabs(elOoEmooPs_data->at(1)) < 0.1443) elooEmooPLoosecut2data = true;

      if(fabs(elOoEmooPs_data->at(1)) < 0.1126) elooEmooPTightcut2data = true;

      //pfIsoCuts
      if(elRelIsos_data->at(1) < 0.3529) elPFIsoLoosecut2data = true;

      if(elRelIsos_data->at(1) < 0.2075) elPFIsoTightcut2data = true;

      //missing hits cuts
      if(elMHits_data->at(1) <= 1) elMissingHitsLoosecut2data = true;

      if(elMHits_data->at(1) <= 1) elMissingHitsTightcut2data = true;

    }
    
    //now add them all up
    elTight1 = (elDPhiTightcut1data * elDEtaTightcut1data * elSigmaIetaIetaTightcut1data * elHETightcut1data * elD0Tightcut1data * elDZTightcut1data * elooEmooPTightcut1data * elPFIsoTightcut1data * elMissingHitsTightcut1data);
    elTight2 = (elDPhiTightcut2data * elDEtaTightcut2data * elSigmaIetaIetaTightcut2data * elHETightcut2data * elD0Tightcut2data * elDZTightcut2data * elooEmooPTightcut2data * elPFIsoTightcut2data * elMissingHitsTightcut2data);
    elLoose1 = (elDPhiLoosecut1data * elDEtaLoosecut1data * elSigmaIetaIetaLoosecut1data * elHELoosecut1data * elD0Loosecut1data * elDZLoosecut1data * elooEmooPLoosecut1data * elPFIsoLoosecut1data * elMissingHitsLoosecut1data);
    elLoose2 = (elDPhiLoosecut2data * elDEtaLoosecut2data * elSigmaIetaIetaLoosecut2data * elHELoosecut2data * elD0Loosecut2data * elDZLoosecut2data * elooEmooPLoosecut2data * elPFIsoLoosecut2data * elMissingHitsLoosecut2data);
    */
    /*
    if(elTight1){
      std::cout<<"found a tight lepton1"<<std::endl;
    }
    if(elLoose1) std::cout<<"found a loose lepton1"<<std::endl;
    if(elTight2) std::cout<<"found a tight lepton2"<<std::endl;
    if(elLoose2) std::cout<<"found a loose lepton2"<<std::endl;
    */
    //count events and fill histograms

  //  std::cout<<"Number of events passing dilepton cut: "<<Ndilepcut_data<<std::endl;
  //std::cout<<"Number of events passing SS dilepton cut: "<<NSSdilepcut_data<<std::endl;
  std::cout<<"Number of eventspassing mass cut: "<<Nmasscut_data<<std::endl;
  std::cout<<"Number of eventspassing met cut: "<<Nmetcut_data<<std::endl;
  std::cout<<"Number of eventspassing NJets cut: "<<NJetscut_data<<std::endl;
  std::cout<<"Number of eventspassing Jetpt1 cut: "<<NJetpt1cut_data<<std::endl;
  std::cout<<"Number of eventspassing Jetpt2 cut: "<<NJetpt2cut_data<<std::endl;
  std::cout<<"Number of events passing HT cut: "<<NHTcut_data<<std::endl;


  std::cout<<"Number of tight-tight events: "<<Ntt_data<<std::endl;
  std::cout<<"Number of tight-loose events: "<<Ntl_data<<std::endl;
  std::cout<<"Number of loose-loose events: "<<Nll_data<<std::endl;



  //NOW CONVERT THESE INTO HOW MANY PASSING EVENTS ARE FROM BACKGROUND WITHOUT A PROMPT LEPTON
  
  Npf_data = ( eps_mu / ( (1-eps_e*eta_e) * ( 1 - eps_mu *eta_mu))) * ( (-eps_e * Nll_data) + (Ntl_data) + (eps_e * eta_mu * Nlt_data) - (eta_mu*Ntt_data));

  Nfp_data = ( eps_e / ( (1-eps_e*eta_e) * ( 1 - eps_mu *eta_mu))) * ( (-eps_mu * Nll_data) + (eta_e * eps_mu * Ntl_data) + (Nlt_data) - (eta_e*Ntt_data) );
  
  Nff_data = ( (eps_mu * eps_e) / ( (1 - eps_e*eta_e)*(1 - eps_mu*eta_mu))) * ( (Nll_data) - (eta_e*Ntldata) - (eta_mu*Nlt_data) + (eta_e*eta_mu*Ntt_data));

  std::cout<<"Nfp_pass: "<<Nfp_data<<std::endl;
  std::cout<<"Nff_pass: "<<Nff_data<<std::endl;


  //then, N_fp plus N_ff should equal the number of events we have with two tight leptons so let's check:
  float N_fake = Nfp_data + Nff_data;
  std::cout<<"predicted number of non-prompt events: "<<N_fake<<std::endl;
  std::cout<<"observed number of non-prompt events: "<<Ntt_data<<std::endl;
  /* 
     MONTE CARLO PART. HERE WE WANT TO APPLY ALL THE ANALYSIS CUTS AND SIMPLY SEE HOW MANY EVENTS ACTUALLY DO PASS OUR SELECTION
   */

}
