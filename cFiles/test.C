//Getting rid of the xtalk for front and back for large particles

if(id ==6 && Front.Nstore >1 && Front.Order[0].energy >100.) //Is it a large pulse?
  {
    for(int ff = 1;ff<Front.Nstore;ff++)
      {
	if(abs(Front.Order[0].strip - Front.Order[ff].strip) == 1) //Is it a neighbor?
	  {
	    if(ff != Front.Nstore-1) //Is it the last in the list?
	      {
		for(int i = ff+1;i<Front.Nstore;i++)
		  {
		    Front.Order[i-1] =  Front.Order[i];  //Move everything up.
		  }
	      }
	    Front.Nstore--; //Decrease the number of entries
	    
	  }
      }
  }
