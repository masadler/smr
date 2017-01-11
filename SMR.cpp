//
//  SMR.cpp
//  SRM_CPP
//
//  Created by Futao Zhang on 29/06/15.
//  Copyright (c) 2015 Futao Zhang. All rights reserved.
//


#include "StrFunc.h"
#include "SMR_data_p1.h"
#include "SMR_data_p2.h"
#include "SMR_data_p3.h"
#include "SMR_plot.h"
#include "SMR.h"

using namespace SMRDATA;
using namespace StatFunc;
int thread_num;
int xh=0;

int main(int argc, char** argv) {
 
    cout << "*******************************************************************" << endl;
    cout << "* Summary-data-based Mendelian Randomization (SMR)" << endl;
    cout << "* version 0.65" << endl;
    cout << "* (C) 2015 Futao Zhang, Zhihong Zhu and Jian Yang" << endl;
    cout << "* The University of Queensland" << endl;
    cout << "* MIT License" << endl;
    cout << "*******************************************************************" << endl;
   
    FLAGS_VALID_CK(argc,argv);
    long int time_used = 0, start = time(NULL);
    time_t curr = time(0);
    cout << "Analysis started: " << ctime(&curr) << endl;
    cout << "Options:" << endl;
    try {
        option(argc, argv);
    } catch (const string &err_msg) {
        cerr << "\n" << err_msg << endl;
    } catch (const char *err_msg) {
        cerr << "\n" << err_msg << endl;
    }    
    curr = time(0);
    cout << "\nAnalysis finished: " << ctime(&curr);
    time_used = time(NULL) - start;
    cout << "Computational time: " << time_used / 3600 << ":" << (time_used % 3600) / 60 << ":" << time_used % 60 << endl;
    
    return 0;
}

void option(int option_num, char* option_str[])
{
    char* bFileName=NULL;
    char* gwasFileName=NULL;
    char* eqtlFileName=NULL;
    char* outFileName=NULL;
    char* indilstName=NULL;
    char* snplstName=NULL;
    char* indilst2remove=NULL;
    char* snplst2exclde=NULL;
    char* problst2exclde=NULL;
    char* eproblst2exclde=NULL;
     char* oproblst2exclde=NULL;
    bool bFlag=false;// for binary file
    double maf=0.0;
    double p_hetero=1.5654e-3;
    double p_smr=5.0e-8;
    double ld_prune=0.9;
    unsigned int m_hetero=3;
    bool smr_flag=true;
    bool smr_trans_flag=false;
    
    // for data management
    bool make_besd_flag=false;
    char* problstName=NULL;
    char* eproblstName=NULL;
    char* oproblstName=NULL;
	
    char* eFileName=NULL;
    bool eremlFlag=false;
    
    // for filtering
    bool cis_flag=false;
    int cis_itvl=2000;
    int trans_itvl=1000;
    float transThres=5.0e-8;
    float restThres=1.0e-5;
    
    // for lookup
    double plookup=5e-8;
    bool lookup_flag=false;
    char* genelistName=NULL;
    int chr=0;
    int prbchr=0;
    int snpchr=0;
    char* snprs=NULL;
    char* prbname=NULL;
    char* fromsnprs=NULL;
    char* tosnprs=NULL;
    char* fromprbname=NULL;
    char* toprbname=NULL;
    int snpWind=50;   //Kb
    int prbWind=1000; //Kb
    char* genename=NULL;
    int fromsnpkb=-9;
    int tosnpkb=-9;
    int fromprbkb=-9;
    int toprbkb=-9;
    bool snpwindFlag=false;
    bool prbwindFlag=false;
    
    
    char* refSNP=NULL;
    bool heidioffFlag = false;
    
    thread_num = 1;
    
    bool combineFlg = false;
    char* eqtlsmaslstName = NULL;
    //
    char* gwasFileName2=NULL;
    char* eqtlFileName2=NULL;
    char* traitlstName=NULL;
    //
    bool plotflg=false;
    //
    char* syllabusName=NULL;
    bool gctaflag=false;
    bool plinkflag=false;
    bool gemmaflag=false;
    bool merlinflag=false;
    bool boltflag=false;
    
    bool genouni=false;
    
    
    char* freqName=NULL;
    bool esdstd=false;
    char* vpFileName=NULL;
    
    bool metaflg=false;
    bool est_effe_spl_size_flg=false;
    // for SMR e2me
    int outcomePrbWind=2000; //kb
    char* eprobe=NULL;
    char* oprobe=NULL;
    char* eprobe2rm=NULL;
    char* oprobe2rm=NULL;
    
    // for internal test
    char* smrRltFileName = NULL;
    bool interanlflg=false;
    
    bool recodeflg=false;
    // for setBased SMR
    char* geneAnnoName=NULL;
    char* setlstName=NULL;
    int setWind=-9;
    bool ssmrflg=false;
    
    //
    char* queryFileName=NULL;
    bool queryfileflg=false;
    
    bool save_dense_flag=false;
    
    double pthres_me2esmr=1.0;
    bool diffflag=false;
    
    double threshpsmrest=1.0;
    double threshphet=0.0;
    bool qcflag=false;
    int qcmtd=0;
    int z_thresh=3;
    bool new_het_mth=true;
    
    
    bool make_query=false;
    char* f1FileName=NULL;
    char* f2FileName=NULL;
    
    bool count_cis_flag = false;
    bool count_trans_flag = false;
    
    for(int i=0;i<option_num;i++)
    {
        // Plink files for LD from a reference sample
        if(0==strcmp(option_str[i],"--bfile")){
            bFileName=option_str[++i];
            FLAG_VALID_CK("--bfile", bFileName);
            printf("--bfile %s\n",bFileName);
        }
        // gwas data file as cojo format
        else if(0==strcmp(option_str[i],"--gwas-summary")){
            if(gwasFileName ==NULL){
                gwasFileName=option_str[++i];
                FLAG_VALID_CK("--gwas-summary", gwasFileName);
                printf("--gwas-summary %s\n",gwasFileName);
                CommFunc::FileExist(gwasFileName);
            }else
            {
                gwasFileName2=option_str[++i];
                FLAG_VALID_CK("--gwas-summary", gwasFileName2);
                printf("--gwas-summary %s\n",gwasFileName2);
                CommFunc::FileExist(gwasFileName2);
            }
        }
        // eQTL files
        else if(0==strcmp(option_str[i],"--eqtl-summary")){
            if(eqtlFileName==NULL)
            {
                eqtlFileName=option_str[++i];
                FLAG_VALID_CK("--eqtl-summary", eqtlFileName);
                printf("--eqtl-summary %s\n",eqtlFileName);
            }else{
                eqtlFileName2=option_str[++i];
                FLAG_VALID_CK("--eqtl-summary", eqtlFileName2);
                printf("--eqtl-summary %s\n",eqtlFileName2);
            }
           
            
        }
        else if(0==strcmp(option_str[i],"--beqtl-summary")){
             bFlag=true;
            if(eqtlFileName==NULL)
            {
                eqtlFileName=option_str[++i];
                FLAG_VALID_CK("--beqtl-summary", eqtlFileName);
                printf("--beqtl-summary %s\n",eqtlFileName);
            }else{
                eqtlFileName2=option_str[++i];
                FLAG_VALID_CK("--beqtl-summary", eqtlFileName2);
                printf("--beqtl-summary %s\n",eqtlFileName2);
            }
        }
        else if(strcmp(option_str[i],"--keep")==0){
            indilstName=option_str[++i];
            FLAG_VALID_CK("--keep", indilstName);
            cout<<"--keep "<<indilstName<<endl;
            CommFunc::FileExist(indilstName);
        }
        else if(strcmp(option_str[i],"--remove")==0){
            indilst2remove=option_str[++i];
            FLAG_VALID_CK("--remove", indilst2remove);
            cout<<"--remove "<<indilst2remove<<endl;
            CommFunc::FileExist(indilst2remove);
        }
        else if(strcmp(option_str[i],"--extract-snp")==0){
            snplstName=option_str[++i];
            FLAG_VALID_CK("--extract-snp", snplstName);
            cout<<"--extract-snp "<<snplstName<<endl;
            CommFunc::FileExist(snplstName);
        }
        else if(strcmp(option_str[i],"--extract-probe")==0){
            problstName=option_str[++i];
            FLAG_VALID_CK("--extract-probe", problstName);
            cout<<"--extract-probe "<<problstName<<endl;
            CommFunc::FileExist(problstName);
        }
        else if(strcmp(option_str[i],"--extract-exposure-probe")==0){
            eproblstName=option_str[++i];
            FLAG_VALID_CK("--extract-exposure-probe", eproblstName);
            cout<<"--extract-exposure-probe "<<eproblstName<<endl;
            CommFunc::FileExist(eproblstName);
        }
        else if(strcmp(option_str[i],"--extract-outcome-probe")==0){
            oproblstName=option_str[++i];
            FLAG_VALID_CK("--extract-outcome-probe", oproblstName);
            cout<<"--extract-outcome-probe "<<oproblstName<<endl;
            CommFunc::FileExist(oproblstName);
        }
        else if(strcmp(option_str[i],"--exclude-snp")==0){
            snplst2exclde=option_str[++i];
            FLAG_VALID_CK("--exclude-snp", snplst2exclde);
            cout<<"--exclude-snp "<<snplst2exclde<<endl;
            CommFunc::FileExist(snplst2exclde);
        }
        else if(strcmp(option_str[i],"--exclude-probe")==0){
            problst2exclde=option_str[++i];
            FLAG_VALID_CK("--exclude-probe", problst2exclde);
            cout<<"--exclude-probe "<<problst2exclde<<endl;
            CommFunc::FileExist(problst2exclde);
        }
        else if(strcmp(option_str[i],"--exclude-exposure-probe")==0){
            eproblst2exclde=option_str[++i];
            FLAG_VALID_CK("--exclude-exposure-probe", eproblst2exclde);
            cout<<"--exclude-exposure-probe "<<eproblst2exclde<<endl;
            CommFunc::FileExist(eproblst2exclde);
        }
        else if(strcmp(option_str[i],"--exclude-outcome-probe")==0){
            oproblst2exclde=option_str[++i];
            FLAG_VALID_CK("--exclude-outcome-probe", oproblst2exclde);
            cout<<"--exclude-outcome-probe "<<oproblst2exclde<<endl;
            CommFunc::FileExist(oproblst2exclde);
        }

        else if(strcmp(option_str[i],"--maf")==0){
            maf=atof(option_str[++i]);
            cout<<"--maf "<<maf<<endl;
            if(maf<0 || maf>0.5)
            {
                fprintf (stderr, "Error: --maf should be within the range from 0 to 0.5.\n");
                exit (EXIT_FAILURE);
            }
        }
        else if(strcmp(option_str[i],"--make-besd")==0){
            make_besd_flag=true;
            cout<<"--make-besd "<<endl;
        }
        else if (0 == strcmp(option_str[i], "--out")){
            outFileName = option_str[++i];
            if(outFileName !=NULL && has_prefix(outFileName, "--"))
            {
                outFileName=NULL;
                i--;
            }else   printf("--out %s\n", outFileName);
        }
        else if (0 == strcmp(option_str[i], "--peqtl-smr")){
            p_smr = atof(option_str[++i]);
            printf("--peqtl-smr %10.2e\n", p_smr);
            if(p_smr<0 || p_smr>1)
            {
                fprintf (stderr, "Error: --peqtl-smr should be within the range from 0 to 1.\n");
                exit (EXIT_FAILURE);
            }
        }
        else if (0 == strcmp(option_str[i], "--peqtl-heidi")){
            p_hetero = atof(option_str[++i]);
            printf("--peqtl-heidi %10.2e\n", p_hetero);
            if(p_hetero<0 || p_hetero>1)
            {
                fprintf (stderr, "Error: --peqtl-heidi should be within the range from 0 to 1.\n");
                exit (EXIT_FAILURE);
            }
            
        }
        else if (0 == strcmp(option_str[i], "--heidi-m")){
            m_hetero = atoi(option_str[++i]);
            printf("--heidi-m %d\n", m_hetero);
        }
        else if (0 == strcmp(option_str[i], "--heidi-mtd")){
            int mtd=atoi(option_str[++i]);
            if(mtd==0) new_het_mth=false;
            else if(mtd==1) new_het_mth=true;
            else {
                printf("ERROR: only tow HEIDI methold avaliable.\n");
                exit(EXIT_FAILURE);
            }
            printf("--heidi-mtd %d\n", mtd);
        }
        else if (0 == strcmp(option_str[i], "--ld-pruning")){
            ld_prune = atof(option_str[++i]);
            printf("--ld-pruning %f\n", ld_prune);
            if(ld_prune<0 || ld_prune>1)
            {
                fprintf (stderr, "Error: --ld-pruning should be within the range from 0 to 1.\n");
                exit (EXIT_FAILURE);
            }
        }
        else if (0 == strcmp(option_str[i], "--smr")){
            smr_flag=true;
            printf("--smr \n");
        }
        else if (0 == strcmp(option_str[i], "--smr-multi")){
            ssmrflg=true;
            printf("--smr-multi \n");
        }
        else if (0 == strcmp(option_str[i], "--make-besd-dense")){
            make_besd_flag=true;
            save_dense_flag=true;
            printf("--make-besd-dense \n");
        }
        else if(strcmp(option_str[i],"--cis-wind")==0){
            cis_flag=true;
            cis_itvl=atoi(option_str[++i]);
            printf("--cis-wind %d Kb\n", cis_itvl);
            if(cis_itvl<0 )
            {
                fprintf (stderr, "Error: --cis-wind should be over 0.\n");
                exit (EXIT_FAILURE);
            }
        }
        else if(strcmp(option_str[i],"--trans-wind")==0){
            trans_itvl=atoi(option_str[++i]);
            printf("--trans-wind %d Kb\n", trans_itvl);
            if(trans_itvl<0 )
            {
                fprintf (stderr, "Error: --trans-wind should be over 0.\n");
                exit (EXIT_FAILURE);
            }
        }
        else if(strcmp(option_str[i],"--peqtl-trans")==0){
            transThres=atof(option_str[++i]);
            printf("--peqtl-trans %10.2e\n", transThres);
            if(transThres<0 || transThres>1)
            {
                fprintf (stderr, "Error: --peqtl-trans should be within the range from 0 to 1.\n");
                exit (EXIT_FAILURE);
            }
        }
        else if(strcmp(option_str[i],"--peqtl-other")==0){
            restThres=atof(option_str[++i]);
            printf("--peqtl-other %10.2e\n", restThres);
            if(restThres<0 || restThres>1)
            {
                fprintf (stderr, "Error: --peqtl-other should be within the range from 0 to 1.\n");
                exit (EXIT_FAILURE);
            }
        }
        else if (0 == strcmp(option_str[i], "--efile")){
            eremlFlag=true;
            eFileName = option_str[++i];
            FLAG_VALID_CK("--efile", eFileName);
            printf("--efile %s\n", eFileName);
        }
        else if (0 == strcmp(option_str[i], "--query")){
            lookup_flag=true;
            if(i+1==option_num || has_prefix(option_str[i+1],"--"))  plookup = 5.0e-8;
            else plookup = atof (option_str[++i]);
            printf("--query %10.2e\n", plookup);
            if(plookup<0 || plookup>1)
            {
                fprintf (stderr, "Error: --query should be within the range from 0 to 1.\n");
                exit (EXIT_FAILURE);
            }
        }
        else if(strcmp(option_str[i],"--genes")==0){
            genelistName=option_str[++i];
            FLAG_VALID_CK("--genes", genelistName);
            cout<<"--genes "<<genelistName<<endl;
            CommFunc::FileExist(genelistName);
        }
        else if(strcmp(option_str[i],"--gene-list")==0){
            geneAnnoName=option_str[++i];
            FLAG_VALID_CK("--gene-list", geneAnnoName);
            cout<<"--gene-list "<<geneAnnoName<<endl;
            CommFunc::FileExist(geneAnnoName);
        }
        else if(strcmp(option_str[i],"--set-list")==0){
            setlstName=option_str[++i];
            FLAG_VALID_CK("--set-list", setlstName);
            cout<<"--set-list "<<setlstName<<endl;
            CommFunc::FileExist(setlstName);
        }
        else if (0 == strcmp(option_str[i], "--target-snp")){
            refSNP = option_str[++i];
            FLAG_VALID_CK("--target-snp", refSNP);
            printf("--target-snp %s\n", refSNP);
        }
        else if (0 == strcmp(option_str[i], "--heidi-off")){
            heidioffFlag = true;            
            printf("--heidi-off \n");
        }
        else if(0 == strcmp(option_str[i],"--thread-num")){
            thread_num=atoi(option_str[++i]);
            printf("--thread-num %d\n", thread_num);
        }       
        else if (0 == strcmp(option_str[i], "--besd-flist")){
            combineFlg=true;
            eqtlsmaslstName = option_str[++i];
            FLAG_VALID_CK("--besd-flist", eqtlsmaslstName);
            printf("--besd-flist %s\n", eqtlsmaslstName);
        }
        else if (0 == strcmp(option_str[i], "--plot")){
            plotflg = true;
            printf("--plot \n" );
        }
        else if (0 == strcmp(option_str[i], "--trans")){
            smr_trans_flag = true;
            printf("--trans \n" );
        }
        else if (0 == strcmp(option_str[i], "--eqtl-flist")){
            syllabusName = option_str[++i];
             gctaflag = true;
            if(syllabusName !=NULL && has_prefix(syllabusName, "--"))
            {
                syllabusName=NULL;
                i--;
            }else   printf("--eqtl-flist %s\n", syllabusName);
        }
        else if (0 == strcmp(option_str[i], "--smr-format")){
            gctaflag = true;
            printf("--smr-format \n" );
        }
        else if (0 == strcmp(option_str[i], "--plink-qassoc-format")){
            plinkflag = true;
             gctaflag = false;
            printf("--plink-qassoc-format \n" );
        }
        else if (0 == strcmp(option_str[i], "--gemma-format")){
            gemmaflag = true;
            gctaflag = false;
            printf("--gemma-format \n" );
        }
        else if (0 == strcmp(option_str[i], "--bolt-assoc-format")){
            boltflag = true;
            gctaflag = false;
            printf("--bolt-assoc-format \n" );
        }
        else if (0 == strcmp(option_str[i], "--geno-uni")){
            genouni = true;
            printf("--geno-uni \n" );
        }
        else if (0 == strcmp(option_str[i], "--merlin-fastassoc-format")){
            merlinflag = true;
            gctaflag = false;
            printf("----merlin-fastassoc-format \n" );
        }
        else if(strcmp(option_str[i],"--extract-trait")==0){
            traitlstName=option_str[++i];
            FLAG_VALID_CK("--extract-trait", traitlstName);
            cout<<"--extract-trait "<<traitlstName<<endl;
            CommFunc::FileExist(traitlstName);
        }
        else if(strcmp(option_str[i],"--esd-std")==0){
            esdstd=true;
            cout<<"--esd-std "<<endl;
        }
        else if(strcmp(option_str[i],"--freq")==0){
            freqName=option_str[++i];
            FLAG_VALID_CK("--freq", freqName);
            cout<<"--freq "<<freqName<<endl;
            CommFunc::FileExist(freqName);
        }
        else if(strcmp(option_str[i],"--probe-var")==0){
            vpFileName=option_str[++i];
            FLAG_VALID_CK("--probe-var", vpFileName);
            cout<<"--probe-var "<<vpFileName<<endl;
            CommFunc::FileExist(vpFileName);
        }
        else if(strcmp(option_str[i],"--meta")==0){
            metaflg=true;
            cout<<"--meta "<<endl;
        }
        else if(strcmp(option_str[i],"--smr-wind")==0){
            outcomePrbWind=atoi(option_str[++i]);
            printf("--smr-wind %d Kb\n", outcomePrbWind);
            if(outcomePrbWind<0 )
            {
                fprintf (stderr, "Error: --smr-wind should be over 0.\n");
                exit (EXIT_FAILURE);
            }
        }
        else if(strcmp(option_str[i],"--set-wind")==0){
            setWind=atoi(option_str[++i]);
            printf("--set-wind %d kb\n", setWind);
            if(setWind<0 )
            {
                fprintf (stderr, "Error: --set-wind should be over 0.\n");
                exit (EXIT_FAILURE);
            }
        }
        else if(strcmp(option_str[i],"--est-n")==0){
            est_effe_spl_size_flg=true;
            combineFlg=false;
            cout<<"--est-n "<<endl;
        }
        else if(strcmp(option_str[i],"--internal-test")==0){
            interanlflg=true;
            cout<<"--internal-test "<<endl;
        }
        else if(strcmp(option_str[i],"--smr-file")==0){
            smrRltFileName=option_str[++i];
            FLAG_VALID_CK("--smr-file", smrRltFileName);
            cout<<"--smr-file "<<smrRltFileName<<endl;
            CommFunc::FileExist(smrRltFileName);
        }
        else if(strcmp(option_str[i],"--recode")==0){
            recodeflg=true;
            cout<<"--recode "<<endl;
        }
        else if(strcmp(option_str[i],"--chr")==0){
            char* tmpstr=option_str[++i];
            if(strncmp(tmpstr,"X",1)==0) chr=23;
            else if(strncmp(tmpstr,"Y",1)==0) chr=24;
            else chr=atoi(tmpstr);
            FLAG_VALID_CK("--chr", tmpstr);
            cout<<"--chr "<<tmpstr<<endl;
        }
        else if(strcmp(option_str[i],"--probe-chr")==0){
            char* tmpstr=option_str[++i];
            if(strncmp(tmpstr,"X",1)==0) prbchr=23;
            else if(strncmp(tmpstr,"Y",1)==0) prbchr=24;
            else prbchr=atoi(tmpstr);
            FLAG_VALID_CK("--probe-chr", tmpstr);
            cout<<"--probe-chr "<<tmpstr<<endl;
        }
        else if(strcmp(option_str[i],"--snp-chr")==0){
            char* tmpstr=option_str[++i];
            if(strncmp(tmpstr,"X",1)==0) snpchr=23;
            else if(strncmp(tmpstr,"Y",1)==0) snpchr=24;
            else snpchr=atoi(tmpstr);
            FLAG_VALID_CK("--snp-chr", tmpstr);
            cout<<"--snp-chr "<<tmpstr<<endl;
        }
        else if (0 == strcmp(option_str[i], "--snp")){
            snprs = option_str[++i];
            FLAG_VALID_CK("--snp", snprs);
            printf("--snp %s\n", snprs);
        }
        else if (0 == strcmp(option_str[i], "--from-snp")){
            fromsnprs = option_str[++i];
            FLAG_VALID_CK("--from-snp", fromsnprs);
            printf("--from-snp %s\n", fromsnprs);
        }
        else if (0 == strcmp(option_str[i], "--to-snp")){
            tosnprs = option_str[++i];
            FLAG_VALID_CK("--to-snp", tosnprs);
            printf("--to-snp %s\n", tosnprs);
        }
        else if (0 == strcmp(option_str[i], "--probe")){
            prbname = option_str[++i];
            FLAG_VALID_CK("--probe", prbname);
            printf("--probe %s\n", prbname);
        }
        else if (0 == strcmp(option_str[i], "--from-probe")){
            fromprbname = option_str[++i];
            FLAG_VALID_CK("--from-probe", fromprbname);
            printf("--from-probe %s\n", fromprbname);
        }
        else if (0 == strcmp(option_str[i], "--to-probe")){
            toprbname = option_str[++i];
            FLAG_VALID_CK("--to-probe", toprbname);
            printf("--to-probe %s\n", toprbname);
        }
        else if(strcmp(option_str[i],"--snp-wind")==0){
            snpwindFlag=true;
            char* tmpstr=option_str[++i];
            if(tmpstr==NULL || SMRDATA::has_prefix(tmpstr, "--")) i--;
            else snpWind=atoi(tmpstr);
            printf("--snp-wind %d Kb\n", snpWind);
            if(snpWind<0 )
            {
                fprintf (stderr, "Error: --snp-wind should be over 0.\n");
                exit (EXIT_FAILURE);
            }
        }
        else if(strcmp(option_str[i],"--probe-wind")==0){
            prbwindFlag=true;
            char* tmpstr=option_str[++i];
            if(tmpstr==NULL || SMRDATA::has_prefix(tmpstr, "--")) i--;
            else prbWind=atoi(tmpstr);
            printf("--probe-wind %d Kb\n", prbWind);
            if(prbWind<0 )
            {
                fprintf (stderr, "Error: --probe-wind should be over 0.\n");
                exit (EXIT_FAILURE);
            }
        }
        else if (0 == strcmp(option_str[i], "--gene")){
            genename = option_str[++i];
            FLAG_VALID_CK("--gene", genename);
            printf("--gene %s\n", genename);
        }
        else if(strcmp(option_str[i],"--from-snp-kb")==0){
            fromsnpkb=atoi(option_str[++i]);
            printf("--from-snp-kb %d Kb\n", fromsnpkb);
            if(fromsnpkb<0 )
            {
                fprintf (stderr, "Error: --from-snp-kb should be over 0.\n");
                exit (EXIT_FAILURE);
            }
        }
        else if(strcmp(option_str[i],"--to-snp-kb")==0){
            tosnpkb=atoi(option_str[++i]);
            printf("--to-snp-kb %d Kb\n", tosnpkb);
            if(tosnpkb<0 )
            {
                fprintf (stderr, "Error: --to-snp-kb should be over 0.\n");
                exit (EXIT_FAILURE);
            }
        }
        else if(strcmp(option_str[i],"--from-probe-kb")==0){
            fromprbkb=atoi(option_str[++i]);
            printf("--from-probe-kb %d Kb\n", fromprbkb);
            if(fromprbkb<0 )
            {
                fprintf (stderr, "Error: --from-probe-kb should be over 0.\n");
                exit (EXIT_FAILURE);
            }
        }
        else if(strcmp(option_str[i],"--to-probe-kb")==0){
            toprbkb=atoi(option_str[++i]);
            printf("--to-probe-kb %d Kb\n", toprbkb);
            if(toprbkb<0 )
            {
                fprintf (stderr, "Error: --to-probe-kb should be over 0.\n");
                exit (EXIT_FAILURE);
            }
        }
        else if(strcmp(option_str[i],"--extract-single-exposure-probe")==0){
            eprobe=option_str[++i];
            FLAG_VALID_CK("--extract-single-exposure-probe", eprobe);
            cout<<"--extract-single-exposure-probe "<<eprobe<<endl;
        }
        else if(strcmp(option_str[i],"--extract-single-outcome-probe")==0){
            oprobe=option_str[++i];
            FLAG_VALID_CK("--extract-single-outcome-probe", oprobe);
            cout<<"--extract-single-outcome-probe "<<oprobe<<endl;
        }
        else if(strcmp(option_str[i],"--exclude-single-exposure-probe")==0){
            eprobe2rm=option_str[++i];
            FLAG_VALID_CK("--exclude-single-exposure-probe", eprobe2rm);
            cout<<"--exclude-single-exposure-probe "<<eprobe2rm<<endl;
        }
        else if(strcmp(option_str[i],"--exclude-single-outcome-probe")==0){
            oprobe2rm=option_str[++i];
            FLAG_VALID_CK("--exclude-single-outcome-probe", oprobe2rm);
            cout<<"--exclude-single-outcome-probe "<<oprobe2rm<<endl;
        }
        else if(0==strcmp(option_str[i],"--qfile")){
            queryfileflg=true;
            queryFileName=option_str[++i];
            FLAG_VALID_CK("--qfile", queryFileName);
            printf("--qfile %s\n",queryFileName);
        }
        else if(strcmp(option_str[i],"--diff")==0){
            diffflag=true;
            cout<<"--diff "<<endl;
        }
        else if(strcmp(option_str[i],"--beqtl-qc")==0){
            qcflag=true;
            qcmtd = atoi(option_str[++i]);
            if(qcmtd<0 || qcmtd>2)
            {
                fprintf (stderr, "Error: --beqtl-qc should be 0,1 or 2.\n"); // 1 for SD, 2 for MAD
                exit (EXIT_FAILURE);
            }
            cout<<"--beqtl-qc "<<qcmtd<<endl;
        }
        else if(strcmp(option_str[i],"--z-thresh")==0){
           
            z_thresh = atoi(option_str[++i]);
            if(z_thresh<0 )
            {
                fprintf (stderr, "Error: --z-thresh should be positive.\n");
                exit (EXIT_FAILURE);
            }
            cout<<"--z-thresh "<<z_thresh<<endl;
        }
        else if (0 == strcmp(option_str[i], "--psmr")){
            threshpsmrest = atof(option_str[++i]);
            printf("--psmr %10.2e\n", threshpsmrest);
            if(threshpsmrest<0 || threshpsmrest>1)
            {
                fprintf (stderr, "Error: --psmr should be within the range from 0 to 1.\n");
                exit (EXIT_FAILURE);
            }
        }
        else if (0 == strcmp(option_str[i], "--phet")){
            threshphet = atof(option_str[++i]);
            printf("--phet %10.2e\n", threshphet);
            if(threshphet<0 || threshphet>1)
            {
                fprintf (stderr, "Error: --phet should be within the range from 0 to 1.\n");
                exit (EXIT_FAILURE);
            }
        }
        else if(0==strcmp(option_str[i],"--make-query-rug.nl")){
            make_query=true;
            printf("--make-query-rug.nl %s\n",queryFileName);
        }
        else if(0==strcmp(option_str[i],"--f1-raw-rug.nl")){
            f1FileName=option_str[++i];
            FLAG_VALID_CK("--f1-raw-rug.nl", f1FileName);
            printf("--f1-raw-rug.nl %s\n",f1FileName);
        }
        else if(0==strcmp(option_str[i],"--f2-af-rug.nl")){
            f2FileName=option_str[++i];
            FLAG_VALID_CK("--f2-af-rug.nl", f2FileName);
            printf("--f2-af-rug.nl %s\n",f2FileName);
        }
        else if(0==strcmp(option_str[i],"--count-cis")){
            count_cis_flag=true;
            printf("--count-cis\n");
        }
        else if(0==strcmp(option_str[i],"--count-trans")){
            count_trans_flag=true;
            printf("--count-trans\n");
        }
    }
    
#ifndef __APPLE__
#if defined _WIN64 || defined _WIN32
    omp_set_num_threads(thread_num);
#else
    stringstream ss;
    ss << thread_num;
    setenv("OMP_NUM_THREADS", ss.str().c_str(), 1);
    omp_set_num_threads(thread_num);
    
#endif
#endif
    
    cout<<endl;
    char tmpch[4]="smr";
    if(outFileName == NULL) outFileName=tmpch;
    if(make_besd_flag && (gctaflag || plinkflag || gemmaflag || boltflag) ) make_besd(outFileName, syllabusName, gctaflag, plinkflag, gemmaflag,merlinflag,boltflag,save_dense_flag, cis_itvl,  trans_itvl,  transThres, restThres,genouni); // from text to besd
    else if(make_query) make_qfile(outFileName,  f1FileName,  f2FileName);
    else if (make_besd_flag && queryfileflg) make_besd_byQfile(queryFileName,outFileName, save_dense_flag, cis_itvl,  trans_itvl,  transThres,  restThres);
    else if (combineFlg) combineBesd(eqtlsmaslstName, outFileName, save_dense_flag, cis_itvl,trans_itvl, transThres, restThres, genouni);
    else if(save_dense_flag) make_esd_file(outFileName, eqtlFileName, snplstName,problstName,bFlag,make_besd_flag, snplst2exclde, problst2exclde,  cis_itvl,genelistName,  chr, prbchr,  prbname,  fromprbname,  toprbname, prbWind, fromprbkb,  toprbkb, prbwindFlag,  genename, snpchr,  snprs,  fromsnprs,  tosnprs, snpWind, fromsnpkb,  tosnpkb, snpwindFlag, cis_flag);
    else if(make_besd_flag) make_sparse_besd(eqtlFileName, outFileName, cis_itvl,trans_itvl, transThres, restThres,genelistName,  chr, prbchr,  prbname,  fromprbname,  toprbname, prbWind, fromprbkb,  toprbkb, prbwindFlag,  genename, snpchr,  snprs,  fromsnprs,  tosnprs, snpWind, fromsnpkb,  tosnpkb, snpwindFlag, cis_flag, snplstName, problstName,  snplst2exclde,  problst2exclde, qcflag ,qcmtd, z_thresh);
    else if(plotflg && eqtlFileName2 != NULL) plot_triple( outFileName,  bFileName, gwasFileName,  eqtlFileName,eqtlFileName2, maf, indilstName,  snplstName, p_hetero, ld_prune, m_hetero , indilst2remove,  snplst2exclde,  p_smr,  refSNP,  cis_itvl,  prbname,  prbWind, prbwindFlag,  snpchr,  snprs,  fromsnprs,  tosnprs, snpWind, fromsnpkb,  tosnpkb, snpwindFlag, cis_flag,  geneAnnoName,  pthres_me2esmr,threshpsmrest,new_het_mth,threshphet);
    else if(plotflg) plot_newheidi(outFileName, bFileName,gwasFileName, eqtlFileName, maf,indilstName, snplstName,problstName,bFlag,p_hetero,ld_prune,m_hetero, indilst2remove, snplst2exclde, problst2exclde,p_smr,refSNP, heidioffFlag,cis_itvl, genelistName,  chr, prbchr,  prbname,  fromprbname,  toprbname, prbWind, fromprbkb,  toprbkb, prbwindFlag,  genename, snpchr,  snprs,  fromsnprs,  tosnprs, snpWind, fromsnpkb,  tosnpkb, snpwindFlag, cis_flag,geneAnnoName,threshpsmrest,new_het_mth, threshphet);
    else if(diffflag) diff(eqtlFileName, eqtlFileName2);
    else if(!interanlflg && eqtlFileName2 != NULL) smr_e2e(outFileName, bFileName,eqtlFileName, eqtlFileName2, maf,indilstName, snplstName,problstName,oproblstName,eproblstName,bFlag,p_hetero,ld_prune,m_hetero, indilst2remove, snplst2exclde, problst2exclde,oproblst2exclde,eproblst2exclde,p_smr,refSNP, heidioffFlag,cis_itvl,traitlstName,outcomePrbWind,oprobe, eprobe, oprobe2rm, eprobe2rm,threshpsmrest,new_het_mth);
    else if(lookup_flag) lookup(outFileName,eqtlFileName, snplstName, problstName, genelistName, plookup, bFlag, chr, prbchr,snpchr, snprs, fromsnprs, tosnprs, prbname, fromprbname, toprbname,snpWind,prbWind,genename,fromsnpkb,tosnpkb,fromprbkb, toprbkb, snpwindFlag, prbwindFlag,cis_flag, cis_itvl);
    else if(count_cis_flag) count_cis(outFileName,eqtlFileName, p_smr, cis_itvl);
    else if(count_trans_flag) count_trans(outFileName,eqtlFileName, p_smr, cis_itvl);
    else if (freqName) update_freq(eqtlFileName, freqName);
    else if (interanlflg) iternal_test(outFileName, bFileName, eqtlFileName, eqtlFileName2, maf,indilstName, snplstName,problstName, oproblstName,eproblstName, bFlag, p_hetero, ld_prune, m_hetero, indilst2remove, snplst2exclde,  problst2exclde, oproblst2exclde,eproblst2exclde,p_smr,cis_itvl, smrRltFileName);
    else if(recodeflg) make_cojo(outFileName, eqtlFileName, snplstName, snplst2exclde,  problstName,  problst2exclde,  genelistName, bFlag);
    else if(ssmrflg)  smr_multipleSNP( outFileName,  bFileName, gwasFileName,  eqtlFileName,  maf,indilstName,  snplstName,problstName, bFlag, p_hetero, ld_prune, m_hetero , indilst2remove, snplst2exclde,  problst2exclde, p_smr,  refSNP,  heidioffFlag,  cis_itvl, genelistName,  chr, prbchr,  prbname,  fromprbname,  toprbname, prbWind, fromprbkb,  toprbkb, prbwindFlag,  genename, snpchr, snprs,  fromsnprs,  tosnprs, snpWind, fromsnpkb,  tosnpkb, snpwindFlag, cis_flag, setlstName,  geneAnnoName,  setWind);
    else if(smr_flag && !smr_trans_flag) smr(outFileName, bFileName,gwasFileName, eqtlFileName, maf,indilstName, snplstName,problstName,bFlag,p_hetero,ld_prune,m_hetero, indilst2remove, snplst2exclde, problst2exclde,p_smr,refSNP, heidioffFlag,cis_itvl, genelistName,  chr, prbchr,  prbname,  fromprbname,  toprbname, prbWind, fromprbkb,  toprbkb, prbwindFlag,  genename, snpchr,  snprs,  fromsnprs,  tosnprs, snpWind, fromsnpkb,  tosnpkb, snpwindFlag, cis_flag,threshpsmrest,new_het_mth);
    else if (est_effe_spl_size_flg) est_effect_splsize(eqtlsmaslstName,eqtlFileName, snplstName,problstName,snplst2exclde, problst2exclde,p_smr);
    else if (metaflg) meta(outFileName,eqtlFileName, eqtlFileName2);
   // else if (esdstd) standardization(outFileName, eqtlFileName,bFlag,freqName, vpFileName);
    else if(smr_flag && smr_trans_flag) smr_trans_wholeInOne(outFileName, bFileName,gwasFileName, eqtlFileName, maf,indilstName, snplstName,problstName, bFlag,p_hetero,ld_prune,m_hetero, indilst2remove, snplst2exclde, problst2exclde,transThres,refSNP, heidioffFlag,trans_itvl, plotflg);
    
   }
