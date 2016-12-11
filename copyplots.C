void copyplots()
{


  TFile *in = new TFile("root/Ne17.root");
  TFile *out = new TFile("root/ECsI.root","RECREATE");


  if(!in)
    {
      cout << "No data here" << endl;
      return;
    }


  ostringstream outstring;
  string name;

  TDirectoryFile *dirECsI = new TDirectoryFile("ECsI","ECsI");

  for(int i =0;i<56;i++)
    {

      cout << "Finding " << i << endl;


      in->cd();
      outstring.str("");
      outstring <<"dEE/dEE_"<<i;
      name = outstring.str();
      

      TH2I * dee = (TH2I*)in->Get(name.c_str())->Clone(Form("ECsI_%i",i));


      cout << "Writing " << i << endl;
      out->cd();
      dirECsI->cd();
      dee->Write();

      in->cd();
      outstring.str("");
      outstring <<"dEE/Cal_dEE_"<<i;
      name = outstring.str();
      

      TH2I * caldee = (TH2I*)in->Get(name.c_str())->Clone(Form("Cal_ECsI_%i",i));


      cout << "Writing " << i << endl;
      out->cd();
      dirECsI->cd();
      caldee->Write();

    }



  in->Close();
  out->Close();
  return;
}
