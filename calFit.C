/***************************************************
Click on either side of two peaks (smaller peak first) to fit to gaussian and calculate rough calibration
***************************************************/


void calFit()
{
  TFile file("sort.root");


  ofstream ofile("a.dat");

  ostringstream outstring;
  string name;
  //  for(int itele =0;itele <;itele++)
  //  for(int istrip = 0;istrip<32;istrip++)
  //	  ofile << itele << " " << istrip << " " << 1 << " " << 0 << endl;

  TCanvas *mycan = new TCanvas("mycan","mycan",1600,900);
  mycan->SetCrosshair(1);
  mycan->ToggleEventStatus();
  
  for (int itele=5;itele<=5;itele++)
    {
      for (int istrip=0;istrip<32;istrip++)
	{
	  double par[6];
	  double peak1;
	  double peak2;

	  
	  double energy1 = 5.4479; //second and last peaks of 232U chain
	  double energy2 = 8.6328;


	  outstring.str("");
	  //outstring<< "back/raw/EB"<<itele<<"_"<<istrip;
	  outstring<< "front/raw/EF"<<itele<<"_"<<istrip;
	  name = outstring.str();
          TH1I * hist = (TH1I*)file.Get(name.c_str());
	  hist->GetXaxis()->SetRangeUser(300,1500);
          hist->Draw();
          mycan->Modified();
 	  mycan->Update();


	  TMarker * mark;
          double low1,high1;
	  double low2,high2;

	  mark = (TMarker*) mycan->WaitPrimitive("TMarker");
          low1 = mark->GetX();
	  delete mark;
          mark = (TMarker*) mycan->WaitPrimitive("TMarker");
          high1 = mark->GetX();
	  delete mark;

	  mark = (TMarker*) mycan->WaitPrimitive("TMarker");
          low2 = mark->GetX();
	  delete mark;
          mark = (TMarker*) mycan->WaitPrimitive("TMarker");
          high2 = mark->GetX();


	  
	  TF1 *g1    = new TF1("g1","gaus",low1,high1);
	  TF1 *g2    = new TF1("g2","gaus",low2,high2);

	  hist->Fit(g1,"R");
	  hist->Fit(g2,"R+");


	  g1->GetParameters(&par[0]);
	  g2->GetParameters(&par[3]);


	  peak1 = par[1];
	  peak2 = par[4];
	  
	  double slope = (energy2-energy1)/(peak2-peak1);
          double intercept = energy1 - slope*peak1;
	  ofile << itele << " " << istrip << " " << slope << " " << intercept << endl;
	  cout << itele << "." << istrip << " " << peak1 << " " << peak2 << " " << slope << " " << intercept << endl;

	  delete g1;
 	  delete g2;
	  delete hist;
	  
	}
    }
  //  for(int itele =8;itele <14;itele++)
  //  for(int istrip = 0;istrip<32;istrip++)
  //	  ofile << itele << " " << istrip << " " << 1 << " " << 0 << endl;

  ofile.close();

  return;
}
