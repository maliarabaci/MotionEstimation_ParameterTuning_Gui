#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QSizePolicy>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->setFixedSize(1024,600);
    ui->setupUi(this);
    this->setWindowTitle("Motion Estimation Parameter Tuning GUI");

    // Video image
    QPixmap mypix (":/images/videography_icon-01.png");
    ui->videoarea->setPixmap(mypix);

    // Initialize values
    Initialize() ;

    // Initialize mv extractor
    if( ui->comboBox_motionestimation->currentIndex() == 0 ) {

        mv_extractor.SetMotionEstimationMethod(MotionVectorExtractor::MOTION_OPTICAL_FLOW_LK);
        mv_extractor.SetMotionMagnitudeThreshold( m_lk_nmin_mv_size ) ;
    }
    else if( ui->comboBox_motionestimation->currentIndex() == 1 ) {

        mv_extractor.SetMotionEstimationMethod(MotionVectorExtractor::MOTION_OPTICAL_FLOW_FARNEBACK);
        mv_extractor.SetMotionMagnitudeThreshold( m_farneback_nmin_mv_size ) ;
    }
    else if( ui->comboBox_motionestimation->currentIndex() == 2 ) {

        mv_extractor.SetMotionEstimationMethod(MotionVectorExtractor::MOTION_OPTICAL_FLOW_SF);
        mv_extractor.SetMotionMagnitudeThreshold( m_sf_nmin_mv_size ) ;
    }
    else if( ui->comboBox_motionestimation->currentIndex() == 3 ) {

        mv_extractor.SetMotionEstimationMethod(MotionVectorExtractor::MOTION_OPTICAL_FLOW_DF);
        mv_extractor.SetMotionMagnitudeThreshold( m_df_nmin_mv_size ) ;
    }
    else {

        ui->comboBox_motionestimation->setCurrentIndex( 0 ) ;
        mv_extractor.SetMotionEstimationMethod(MotionVectorExtractor::MOTION_OPTICAL_FLOW_LK);
        mv_extractor.SetMotionMagnitudeThreshold( m_lk_nmin_mv_size ) ;
    }
    mv_extractor.SetFrameSkip( 0 );
    ui->stackedWidget->setCurrentIndex( ui->comboBox_motionestimation->currentIndex() );

    tmrTimer = new QTimer(this) ;
    connect(tmrTimer, SIGNAL(timeout()), this, SLOT(ProcessFrameAndUpdateGUI())) ;
    tmrTimer->start(20) ;

    connect(ui->comboBox_motionestimation, SIGNAL(activated(int)),
               ui->stackedWidget, SLOT(setCurrentIndex(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Initialize( void ) {

    Initialize_LK_Values() ;
    Initialize_Farneback_Values() ;
    Initialize_SimpleFlow_Values();
    Initialize_DeepFlow_Values();
}

void MainWindow::Initialize_LK_Values( void ) {

    m_lk_nmax_corner = ui->LK_maxCorner->value() ;
    m_lk_dquality_level = (double) ui->LK_qualityLevel->value() / 100 ;
    m_lk_nblock_size_slider = ui->LK_BlockSize->value() ;
    m_lk_buse_harris_slider = false ;
    m_lk_dharris_k_slider = (double) ui->LK_Harris_k->value() / 100 ;
    m_lk_nwin_size_slider = ui->LK_windowSize->value() ;
    m_lk_nmax_level_slider = ui->LK_maxPyramidLevel->value() ;
    m_lk_nmax_count_slider =ui->LK_maxCountLevel->value() ;
    m_lk_depsilon_slider = (double) ui->LK_epsilon->value() / 100 ;
    m_lk_dmin_eig_threshold_slider = (double) ui->LK_minEigThreshold->value() / 1000 ;
    m_lk_nmin_mv_size = ui->LK_setMinMVSize->value() ;
}

void MainWindow::Initialize_Farneback_Values( void ) {

    m_farneback_nstep_size = ui->Farneback_stepSize->value() ;
    m_farneback_dpyr_scale = (double) ui->Farneback_maxPyramidScale->value() / 10 ;
    m_farneback_npyr_level = ui->Farneback_pyramidLevel->value() ;
    m_farneback_nwin_size = ui->Farneback_winSize->value() ;
    m_farneback_niterations = ui->Farneback_iterations->value() ;
    m_farneback_npoly_n = ui->Farneback_poly_n->value() ;
    m_farneback_dpoly_sigma = (double) ui->Farneback_poly_sigma->value() / 10 ;
    m_farneback_nmin_mv_size = ui->Farneback_setMinMVSize->value() ;
}

void MainWindow::Initialize_SimpleFlow_Values( void ) {

    m_sf_nstep_size = ui->SimpleFlow_stepSize->value() ;
    m_sf_nlayers = ui->SimpleFlow_layers->value() ;
    m_sf_naveraging_block_size = ui->SimpleFlow_averaginBlockSize->value() ;
    m_sf_nmax_flow = ui->SimpleFlow_maxFlow->value() ;
    m_sf_dsigma_dist = (double) ui->SimpleFlow_sigmaDist->value() / 10 ;
    m_sf_dsigma_color = (double) ui->SimpleFlow_sigmaColor->value() / 10 ;
    m_sf_npostprocess_window = ui->SimpleFlow_postWin->value() ;
    m_sf_dsigma_dist_fix = (double) ui->SimpleFlow_flowSigmaDistFix->value() / 10 ;
    m_sf_dsigma_color_fix = (double) ui->SimpleFlow_flowSigmaColorFix->value() / 10 ;
    m_sf_docc_thr = (double) ui->SimpleFlow_occThr->value() / 100 ;
    m_sf_nupscale_averaging_radius = ui->SimpleFlow_upscaleAvgRadius->value() ;
    m_sf_dupscale_sigma_dist = (double) ui->SimpleFlow_upscaleSigmaDist->value() / 10 ;
    m_sf_dupscale_sigma_color = (double) ui->SimpleFlow_upscaleSigmaColor->value() / 10 ;
    m_sf_dspeed_up_thr = (double) ui->SimpleFlow_speedupThreshold->value() / 10 ;
    m_sf_nmin_mv_size = ui->SimpleFlow_setMinMVSize->value() ;
}

void MainWindow::Initialize_DeepFlow_Values( void ) {

    m_df_nstep_size = ui->DeepFlow_stepSize->value() ;
    m_df_nmin_mv_size = ui->DeepFlow_setMinMVSize->value() ;
}

void MainWindow::on_StopResume_clicked()
{
    if( tmrTimer->isActive() == true ) {

        tmrTimer->stop();
        ui->textErrorWarningBrowser->setText("video has been stopped");
    }
    else {

        tmrTimer->start(20) ;
        ui->textErrorWarningBrowser->setText("video has been resumed");
    }
}

void MainWindow::ProcessFrameAndUpdateGUI() {

    std::vector<MotionVectorExtractor::stMotionVector> vec_stmv_frame ;

    cap.read(mat_cur_frame) ;

    if( mat_cur_frame.empty() == true ) return ;

    // Process frame
    SetMVExtractorParamValues() ;
    mv_extractor.ExtractMotionVectors( mat_cur_frame, mat_pre_frame ) ;
    mv_extractor.GetFrameMotionVectors( vec_stmv_frame ) ;
    DrawLine( mat_cur_frame, vec_stmv_frame );
    std::cout << vec_stmv_frame.size() << std::endl ;
    cv::cvtColor( mat_cur_frame, mat_cur_frame, CV_BGR2RGB ) ;
    QImage qimg((uchar*) mat_cur_frame.data, mat_cur_frame.cols, mat_cur_frame.rows, mat_cur_frame.step, QImage::Format_RGB888) ;

    ui->videoarea->setPixmap(QPixmap::fromImage(qimg));
    mat_pre_frame = mat_cur_frame.clone() ;
}

void MainWindow::on_OpenFile_clicked()
{
    on_StopResume_clicked() ;
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "/home/mehmetali/Videos",
                "Video Files (*.mp4)"
                ) ;
    ui->textErrorWarningBrowser->append(filename.toUtf8().constData());
    cap.open(filename.toUtf8().constData()) ;
    if( cap.isOpened() == false ) {

        ui->textErrorWarningBrowser->append("Error: Selected video cannot be opened succesfully");
        return ;
    }

    // Read the first frame and assign to the previous frame
    cap.read(mat_pre_frame) ;
    if( mat_pre_frame.empty() == true ) return ;
    on_StopResume_clicked() ;
}

void MainWindow::on_LK_maxCorner_sliderMoved(int value)
{
    ui->LK_spinBox_maxCorner->setValue( value );
    m_lk_nmax_corner = value ;
}

void MainWindow::on_LK_spinBox_maxCorner_valueChanged(int value)
{
    ui->LK_maxCorner->setValue( value );
    m_lk_nmax_corner = value ;
}

void MainWindow::SetMVExtractorParamValues( void ) {

    switch( ui->comboBox_motionestimation->currentIndex() ) {

    case 0: // LK

        mv_extractor.SetMotionEstimationMethod( MotionVectorExtractor::MOTION_OPTICAL_FLOW_LK );
        mv_extractor.SetLKMaxCorners( m_lk_nmax_corner ) ;
        mv_extractor.SetLKQualityLevel( m_lk_dquality_level ) ;
        mv_extractor.SetLKBlockSize( m_lk_nblock_size_slider ) ;
        mv_extractor.SetLKUseHarrisDetector( m_lk_buse_harris_slider ) ;
        mv_extractor.SetLKHarrisK( m_lk_dharris_k_slider ) ;
        mv_extractor.SetLKWindowSize( m_lk_nwin_size_slider ) ;
        mv_extractor.SetLKMaxLevel( m_lk_nmax_level_slider ) ;
        mv_extractor.SetLKMaxCount( m_lk_nmax_count_slider ) ;
        mv_extractor.SetLKEpsilon( m_lk_depsilon_slider ) ;
        mv_extractor.SetLKMinEigThreshold( m_lk_dmin_eig_threshold_slider ) ;
        mv_extractor.SetMotionMagnitudeThreshold( m_lk_nmin_mv_size ) ;

        break ;
    case 1: // Farneback

        mv_extractor.SetMotionEstimationMethod( MotionVectorExtractor::MOTION_OPTICAL_FLOW_FARNEBACK );
        mv_extractor.SetFarnebackStepSize( m_farneback_nstep_size );
        mv_extractor.SetFarnebackPyramidScale( m_farneback_dpyr_scale );
        mv_extractor.SetFarnebackPyramidLevel( m_farneback_npyr_level );
        mv_extractor.SetFarnebackWinSize( m_farneback_nwin_size );
        mv_extractor.SetFarnebackIteration( m_farneback_niterations );
        mv_extractor.SetFarnebackPolyN( m_farneback_npoly_n );
        mv_extractor.SetFarnebackPolySigma( m_farneback_dpoly_sigma );
        mv_extractor.SetMotionMagnitudeThreshold( m_farneback_nmin_mv_size ) ;

        break ;
    case 2:

        mv_extractor.SetMotionEstimationMethod( MotionVectorExtractor::MOTION_OPTICAL_FLOW_SF );
        mv_extractor.SetSimpleFlowLayers( m_sf_nlayers ) ;
        mv_extractor.SetSimpleFlowAvBlockSize( m_sf_naveraging_block_size ) ;
        mv_extractor.SetSimpleFlowMaxFlow( m_sf_nmax_flow ) ;
        mv_extractor.SetSimpleFlowSigmaDist( m_sf_dsigma_dist ) ;
        mv_extractor.SetSimpleFlowSigmaColor( m_sf_dsigma_color ) ;
        mv_extractor.SetSimpleFlowPostProcessWin( m_sf_npostprocess_window ) ;
        mv_extractor.SetSimpleFlowSigmaDistFix( m_sf_dsigma_dist_fix ) ;
        mv_extractor.SetSimpleFlowSigmaColorFix( m_sf_dsigma_color_fix ) ;
        mv_extractor.SetSimpleFlowOCCThr( m_sf_docc_thr ) ;
        mv_extractor.SetSimpleFlowUpscaleAvRadius( m_sf_nupscale_averaging_radius ) ;
        mv_extractor.SetSimpleFlowUpscaleSigmaDist( m_sf_dupscale_sigma_dist ) ;
        mv_extractor.SetSimpleFlowUpscaleSigmaColor( m_sf_dupscale_sigma_color ) ;
        mv_extractor.SetSimpleFlowSpeedupThreshold( m_sf_dspeed_up_thr ) ;
        mv_extractor.SetMotionMagnitudeThreshold( m_sf_nmin_mv_size ) ;
        break ;
    case 3:

        mv_extractor.SetMotionEstimationMethod( MotionVectorExtractor::MOTION_OPTICAL_FLOW_DF );
        mv_extractor.SetDeepFlowStepSize( m_df_nstep_size ) ;
        mv_extractor.SetMotionMagnitudeThreshold( m_df_nmin_mv_size ) ;
        break ;
    default: // Default is LK
        mv_extractor.SetMotionMagnitudeThreshold( m_lk_nmin_mv_size ) ;
        mv_extractor.SetLKMaxCorners( m_lk_nmax_corner ) ;
        mv_extractor.SetLKQualityLevel( m_lk_dquality_level ) ;
        mv_extractor.SetLKBlockSize( m_lk_nblock_size_slider ) ;
        mv_extractor.SetLKUseHarrisDetector( m_lk_buse_harris_slider ) ;
        mv_extractor.SetLKHarrisK( m_lk_dharris_k_slider ) ;
        mv_extractor.SetLKWindowSize( m_lk_nwin_size_slider ) ;
        mv_extractor.SetLKMaxLevel( m_lk_nmax_level_slider ) ;
        mv_extractor.SetLKMaxCount( m_lk_nmax_count_slider ) ;
        mv_extractor.SetLKEpsilon( m_lk_depsilon_slider ) ;
        mv_extractor.SetLKMinEigThreshold( m_lk_dmin_eig_threshold_slider ) ;
    }
}

void MainWindow::DrawLine( cv::Mat& frame, std::vector<MotionVectorExtractor::stMotionVector>& stmv_frame ) {

    for (unsigned int i = 0; i < stmv_frame.size(); i++) {

        cv::Point p1(stmv_frame[i].m_startPoint.first, stmv_frame[i].m_startPoint.second ) ;
        cv::Point p2(stmv_frame[i].m_endPoint.first, stmv_frame[i].m_endPoint.second ) ;

        cv::line(frame, p1, p2, cv::Scalar(110, 220, 0), 2, 8);
    }
}

void MainWindow::on_LK_qualityLevel_valueChanged(int value)
{
    ui->LK_spinBox_qualityLevel->setValue( value );
    m_lk_dquality_level = (double) value / 100 ;
}

void MainWindow::on_LK_spinBox_qualityLevel_valueChanged(int value)
{
    ui->LK_qualityLevel->setValue( value ) ;
    m_lk_dquality_level = (double) value / 100 ;
}

void MainWindow::on_LK_BlockSize_valueChanged(int value)
{
    ui->LK_spinBox_BlockSize->setValue( value );
    m_lk_nblock_size_slider = value ;
}

void MainWindow::on_LK_spinBox_BlockSize_valueChanged(int value)
{
    ui->LK_BlockSize->setValue( value ) ;
    m_lk_nblock_size_slider = value ;
}

void MainWindow::on_LK_comboBox_currentIndexChanged(int index)
{
    if( index == 0 ) {

        ui->LK_Harris_k->setEnabled( false );
        ui->LK_spinBox_Harris_k->setEnabled( false );
        ui->LK_label_Harris_k->setEnabled( false );
        m_lk_buse_harris_slider = false ;
    }
    else {

        ui->LK_Harris_k->setEnabled( true );
        ui->LK_spinBox_Harris_k->setEnabled( true );
        ui->LK_label_Harris_k->setEnabled( true );
        m_lk_buse_harris_slider = true ;
    }
}

void MainWindow::on_LK_Harris_k_valueChanged(int value)
{
    ui->LK_spinBox_Harris_k->setValue( value );
    m_lk_dharris_k_slider = (double) value / 100 ;
}

void MainWindow::on_LK_spinBox_Harris_k_valueChanged(int value)
{
    ui->LK_Harris_k->setValue( value ) ;
    m_lk_dharris_k_slider = (double) value / 100 ;
}

void MainWindow::on_LK_windowSize_valueChanged(int value)
{
    ui->LK_spinBox_windowSize->setValue( value );
    m_lk_nwin_size_slider = value ;
}

void MainWindow::on_LK_spinBox_windowSize_valueChanged(int value)
{
    ui->LK_windowSize->setValue( value ) ;
    m_lk_nwin_size_slider = value ;
}

void MainWindow::on_LK_maxPyramidLevel_valueChanged(int value)
{
    ui->LK_spinBox_maxPyramidLevel->setValue( value );
    m_lk_nmax_level_slider = value ;
}

void MainWindow::on_LK_spinBox_maxPyramidLevel_valueChanged(int value)
{
    ui->LK_maxPyramidLevel->setValue( value ) ;
    m_lk_nmax_level_slider = value ;
}

void MainWindow::on_LK_maxCountLevel_valueChanged(int value)
{
    ui->LK_spinBox_maxCountLevel->setValue( value );
    m_lk_nmax_count_slider = value ;
}

void MainWindow::on_LK_spinBox_maxCountLevel_valueChanged(int value)
{
    ui->LK_maxCountLevel->setValue( value ) ;
    m_lk_nmax_count_slider = value ;
}

void MainWindow::on_LK_epsilon_valueChanged(int value)
{
    ui->LK_spinBox_epsilon->setValue( value );
    m_lk_depsilon_slider = (double) value / 100 ;
}

void MainWindow::on_LK_spinBox_epsilon_valueChanged(int value)
{
    ui->LK_epsilon->setValue( value ) ;
    m_lk_depsilon_slider = (double) value / 100 ;
}

void MainWindow::on_LK_minEigThreshold_valueChanged(int value)
{
    ui->LK_spinBox_minEigThreshold->setValue( value );
    m_lk_dmin_eig_threshold_slider = (double) value / 1000 ;
}

void MainWindow::on_LK_spinBox_minEigThreshold_valueChanged(int value)
{
    ui->LK_minEigThreshold->setValue( value ) ;
    m_lk_dmin_eig_threshold_slider = (double) value / 1000 ;
}

void MainWindow::on_LK_setMinMVSize_valueChanged(int value)
{
    ui->LK_spinBox_setMinMVSize->setValue( value ) ;
    m_lk_nmin_mv_size = value ;
}

void MainWindow::on_LK_spinBox_setMinMVSize_valueChanged(int value)
{
    ui->LK_setMinMVSize->setValue( value ) ;
    m_lk_nmin_mv_size = value ;
}

void MainWindow::on_Farneback_stepSize_sliderMoved(int value)
{
    ui->Farneback_spinBox_stepSize->setValue( value );
    m_farneback_nstep_size = value ;
}

void MainWindow::on_Farneback_spinBox_stepSize_valueChanged(int value)
{
    ui->Farneback_stepSize->setValue( value );
    m_farneback_nstep_size = value ;
}

void MainWindow::on_Farneback_maxPyramidScale_sliderMoved(int value)
{
    ui->Farneback_spinBox_maxPyramidScale->setValue( value );
    m_farneback_dpyr_scale = (double) value / 10 ;
}

void MainWindow::on_Farneback_spinBox_maxPyramidScale_valueChanged(int value)
{
    ui->Farneback_maxPyramidScale->setValue( value );
    m_farneback_dpyr_scale = (double) value / 10 ;
}

void MainWindow::on_Farneback_pyramidLevel_sliderMoved(int value)
{
    ui->Farneback_spinBox_pyramidLevel->setValue( value );
    m_farneback_npyr_level = value ;
}

void MainWindow::on_Farneback_spinBox_pyramidLevel_valueChanged(int value)
{
    ui->Farneback_pyramidLevel->setValue( value );
    m_farneback_npyr_level = value ;
}

void MainWindow::on_Farneback_winSize_sliderMoved(int value)
{
    ui->Farneback_spinBox_winSize->setValue( value );
    m_farneback_nwin_size = value ;
}

void MainWindow::on_Farneback_spinBox_winSize_valueChanged(int value)
{
    ui->Farneback_winSize->setValue( value );
    m_farneback_nwin_size = value ;
}

void MainWindow::on_Farneback_iterations_sliderMoved(int value)
{
    ui->Farneback_spinBox_iterations->setValue( value );
    m_farneback_niterations = value ;
}

void MainWindow::on_Farneback_spinBox_iterations_valueChanged(int value)
{
    ui->Farneback_iterations->setValue( value );
    m_farneback_niterations = value ;
}

void MainWindow::on_Farneback_poly_n_sliderMoved(int value)
{
    ui->Farneback_spinBox_poly_n->setValue( value );
    m_farneback_npoly_n = value ;
}

void MainWindow::on_Farneback_spinBox_poly_n_valueChanged(int value)
{
    ui->Farneback_poly_n->setValue( value );
    m_farneback_npoly_n = value ;
}

void MainWindow::on_Farneback_poly_sigma_sliderMoved(int value)
{
    ui->Farneback_spinBox_poly_sigma->setValue( value );
    m_farneback_dpoly_sigma = (double) value / 10 ;
}

void MainWindow::on_Farneback_spinBox_poly_sigma_valueChanged(int value)
{
    ui->Farneback_poly_sigma->setValue( value );
    m_farneback_dpoly_sigma = (double) value / 10 ;
}

void MainWindow::on_Farneback_setMinMVSize_sliderMoved(int value)
{
    ui->Farneback_spinBox_setMinMVSize->setValue( value );
    m_farneback_nmin_mv_size = value ;
}

void MainWindow::on_Farneback_spinBox_setMinMVSize_valueChanged(int value)
{
    ui->Farneback_setMinMVSize->setValue( value );
    m_farneback_nmin_mv_size = value ;
}

void MainWindow::on_DeepFlow_stepSize_sliderMoved(int value)
{
    ui->DeepFlow_spinBox_stepSize->setValue( value );
    m_df_nstep_size = value ;
}

void MainWindow::on_DeepFlow_spinBox_stepSize_valueChanged(int value)
{
    ui->DeepFlow_stepSize->setValue( value );
    m_df_nstep_size = value ;
}

void MainWindow::on_DeepFlow_setMinMVSize_sliderMoved(int value)
{
    ui->DeepFlow_spinBox_setMinMVSize->setValue( value );
    m_df_nmin_mv_size = value ;
}

void MainWindow::on_DeepFlow_spinBox_setMinMVSize_valueChanged(int value)
{
    ui->DeepFlow_setMinMVSize->setValue( value );
    m_df_nmin_mv_size = value ;
}

void MainWindow::on_SimpleFlow_stepSize_sliderMoved(int value)
{
    ui->SimpleFlow_spinBox_stepSize->setValue( value );
    m_sf_nstep_size = value ;
}

void MainWindow::on_SimpleFlow_spinBox_stepSize_valueChanged(int value)
{
    ui->SimpleFlow_stepSize->setValue( value );
    m_sf_nstep_size = value ;
}

void MainWindow::on_SimpleFlow_layers_sliderMoved(int value)
{
    ui->SimpleFlow_spinBox_layers->setValue( value );
    m_sf_nlayers = value ;
}

void MainWindow::on_SimpleFlow_spinBox_layers_valueChanged(int value)
{
    ui->SimpleFlow_layers->setValue( value );
    m_sf_nlayers = value ;
}

void MainWindow::on_SimpleFlow_averaginBlockSize_sliderMoved(int value)
{
    ui->SimpleFlow_spinBox_averaginBlockSize->setValue( value );
    m_sf_naveraging_block_size = value ;
}

void MainWindow::on_SimpleFlow_spinBox_averaginBlockSize_valueChanged(int value)
{
    ui->SimpleFlow_averaginBlockSize->setValue( value );
    m_sf_naveraging_block_size = value ;
}

void MainWindow::on_SimpleFlow_maxFlow_sliderMoved(int value)
{
    ui->SimpleFlow_spinBox_maxFlow->setValue( value );
    m_sf_nmax_flow = value ;
}

void MainWindow::on_SimpleFlow_spinBox_maxFlow_valueChanged(int value)
{
    ui->SimpleFlow_maxFlow->setValue( value );
    m_sf_nmax_flow = value ;
}

void MainWindow::on_SimpleFlow_sigmaDist_sliderMoved(int value)
{
    ui->SimpleFlow_spinBox_sigmaDist->setValue( value );
    m_sf_dsigma_dist = (double) value / 10 ;
}

void MainWindow::on_SimpleFlow_spinBox_sigmaDist_valueChanged(int value)
{
    ui->SimpleFlow_sigmaDist->setValue( value );
    m_sf_dsigma_dist = (double) value / 10 ;
}

void MainWindow::on_SimpleFlow_sigmaColor_sliderMoved(int value)
{
    ui->SimpleFlow_spinBox_sigmaColor->setValue( value );
    m_sf_dsigma_color = (double) value / 10 ;
}

void MainWindow::on_SimpleFlow_spinBox_sigmaColor_valueChanged(int value)
{
    ui->SimpleFlow_sigmaColor->setValue( value );
    m_sf_dsigma_color = (double) value / 10 ;
}

void MainWindow::on_SimpleFlow_postWin_sliderMoved(int value)
{
    ui->SimpleFlow_spinBox_postWin->setValue( value );
    m_sf_npostprocess_window = value ;
}

void MainWindow::on_SimpleFlow_spinBox_postWin_valueChanged(int value)
{
    ui->SimpleFlow_postWin->setValue( value );
    m_sf_npostprocess_window = value ;
}

void MainWindow::on_SimpleFlow_flowSigmaDistFix_sliderMoved(int value)
{
    ui->SimpleFlow_spinBox_flowSigmaDistFix->setValue( value );
    m_sf_dsigma_dist_fix = (double) value / 10 ;
}

void MainWindow::on_SimpleFlow_spinBox_flowSigmaDistFix_valueChanged(int value)
{
    ui->SimpleFlow_flowSigmaDistFix->setValue( value );
    m_sf_dsigma_dist_fix = (double) value / 10 ;
}

void MainWindow::on_SimpleFlow_flowSigmaColorFix_sliderMoved(int value)
{
    ui->SimpleFlow_spinBox_flowSigmaColorFix->setValue( value );
    m_sf_dsigma_color_fix = (double) value / 10 ;
}

void MainWindow::on_SimpleFlow_spinBox_flowSigmaColorFix_valueChanged(int value)
{
    ui->SimpleFlow_flowSigmaColorFix->setValue( value );
    m_sf_dsigma_color_fix = (double) value / 10 ;
}

void MainWindow::on_SimpleFlow_occThr_sliderMoved(int value)
{
    ui->SimpleFlow_spinBox_occThr->setValue( value );
    m_sf_docc_thr = (double) value / 100 ;
}

void MainWindow::on_SimpleFlow_spinBox_occThr_valueChanged(int value)
{
    ui->SimpleFlow_occThr->setValue( value );
    m_sf_docc_thr = (double) value / 100 ;
}

void MainWindow::on_SimpleFlow_upscaleAvgRadius_sliderMoved(int value)
{
    ui->SimpleFlow_spinBox_upscaleAvgRadius->setValue( value );
    m_sf_nupscale_averaging_radius = value ;
}

void MainWindow::on_SimpleFlow_spinBox_upscaleAvgRadius_valueChanged(int value)
{
    ui->SimpleFlow_upscaleAvgRadius->setValue( value );
    m_sf_nupscale_averaging_radius = value ;
}

void MainWindow::on_SimpleFlow_upscaleSigmaDist_sliderMoved(int value)
{
    ui->SimpleFlow_spinBox_upscaleSigmaDist->setValue( value );
    m_sf_dupscale_sigma_dist = (double) value / 10 ;
}

void MainWindow::on_SimpleFlow_spinBox_upscaleSigmaDist_valueChanged(int value)
{
    ui->SimpleFlow_upscaleSigmaDist->setValue( value );
    m_sf_dupscale_sigma_dist = (double) value / 10 ;
}

void MainWindow::on_SimpleFlow_upscaleSigmaColor_sliderMoved(int value)
{
    ui->SimpleFlow_spinBox_upscaleSigmaColor->setValue( value );
    m_sf_dupscale_sigma_color = (double) value / 10 ;
}

void MainWindow::on_SimpleFlow_spinBox_upscaleSigmaColor_valueChanged(int value)
{
    ui->SimpleFlow_upscaleSigmaColor->setValue( value );
    m_sf_dupscale_sigma_color = (double) value / 10 ;
}

void MainWindow::on_SimpleFlow_speedupThreshold_sliderMoved(int value)
{
    ui->SimpleFlow_spinBox_speedupThreshold->setValue( value );
    m_sf_dspeed_up_thr = (double) value / 10 ;
}

void MainWindow::on_SimpleFlow_spinBox_speedupThreshold_valueChanged(int value)
{
    ui->SimpleFlow_speedupThreshold->setValue( value );
    m_sf_dspeed_up_thr = (double) value / 10 ;
}

void MainWindow::on_SimpleFlow_setMinMVSize_sliderMoved(int value)
{
    ui->SimpleFlow_spinBox_setMinMVSize_2->setValue( value );
    m_sf_nmin_mv_size = value ;
}

void MainWindow::on_SimpleFlow_spinBox_setMinMVSize_2_valueChanged(int value)
{
    ui->SimpleFlow_setMinMVSize->setValue( value );
    m_sf_nmin_mv_size = value ;
}
