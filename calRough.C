void calRough()
{
  TFile file("sort.root");


  ofstream ofile("a.dat");


  ostringstream outstring;
  string name;
  //  for(int itele =0;itele <;itele++)
  //  for(int istrip = 0;istrip<32;istrip++)
  //	  ofile << itele << " " << istrip << " " << 1 << " " << 0 << endl;

  TCanvas *mycan = new TCanvas("mycan","mycan",800,600);

  for (int itele=0;itele<=13;itele++)
    {
      for (int istrip=0;istrip<32;istrip++)
	{
          outstring.str("");
	  //outstring<< "back/raw/EB"<<itele<<"_"<<istrip;
          outstring<< "front/raw/EF"<<itele<<"_"<<istrip;
	  name = outstring.str();
          TH1I * hist = (TH1I*)file.Get(name.c_str());
	  hist->GetXaxis()->SetRangeUser(200,1500);
          hist->Draw();
          mycan->Modified();
	  mycan->Update();


	  TMarker * mark;
          double low,high;
          mark = (TMarker*) mycan->WaitPrimitive("TMarker");
          low = mark->GetX();
	  delete mark;
          mark = (TMarker*) mycan->WaitPrimitive("TMarker");
          high = mark->GetX();
	  double slope = (8.6328-5.4779)/(high-low);
          double intercept = 5.432 - slope*low;
	  ofile << itele << " " << istrip << " " << slope << " " << intercept << endl;
	  cout << itele << "." << istrip << " " << low << " " << high << endl;
	  
	 
	}
    }
  //  for(int itele =8;itele <14;itele++)
  //  for(int istrip = 0;istrip<32;istrip++)
  //	  ofile << itele << " " << istrip << " " << 1 << " " << 0 << endl;

  ofile.close();

  return;
}
