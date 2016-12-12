{
  gROOT->Reset();
  cut1 = (TCutG*)gPad->WaitPrimitive("CUTG","CutG");
  
  cut1->SetName("cut1");
  cut1->Print();
  int n = cut1->GetN();
  double *x = cut1->GetX();
  double *y = cut1->GetY();
  //open file for output of bananas
  ofstream outfile("banana.dat",ios::out); 
  outfile << n << endl;
  for (int i=0;i<n;i++)outfile << *x++ << " " << *y++ << 
    endl;
  outfile.close();
  delete cut1;
}
