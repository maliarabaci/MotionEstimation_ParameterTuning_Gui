#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#include "MotionVectorExtractor.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    //QStackedWidget* m_pStackedWidget;

    //
    MotionVectorExtractor mv_extractor ;

    // Lucas Kanade parameters
    int m_lk_nmax_corner ;
    double m_lk_dquality_level ;
    int m_lk_nblock_size_slider ;
    int m_lk_buse_harris_slider ;
    double m_lk_dharris_k_slider ;
    int m_lk_nwin_size_slider ;
    int m_lk_nmax_level_slider ;
    int m_lk_nmax_count_slider ;
    double m_lk_depsilon_slider ;
    double m_lk_dmin_eig_threshold_slider ;
    int m_lk_nmin_mv_size ;

    // Lucas Kanade parameters
    int m_farneback_nstep_size ;
    double m_farneback_dpyr_scale ;
    int m_farneback_npyr_level ;
    int m_farneback_nwin_size ;
    int m_farneback_niterations ;
    int m_farneback_npoly_n ;
    double m_farneback_dpoly_sigma ;
    int m_farneback_nmin_mv_size ;

    // SimpleFlow parameters
    int m_sf_nstep_size ;
    int m_sf_nlayers ;
    int m_sf_naveraging_block_size ;
    int m_sf_nmax_flow ;
    double m_sf_dsigma_dist ;
    double m_sf_dsigma_color ;
    int m_sf_npostprocess_window ;
    double m_sf_dsigma_dist_fix ;
    double m_sf_dsigma_color_fix ;
    double m_sf_docc_thr ;
    int m_sf_nupscale_averaging_radius ;
    double m_sf_dupscale_sigma_dist ;
    double m_sf_dupscale_sigma_color ;
    double m_sf_dspeed_up_thr ;
    int m_sf_nmin_mv_size ;

    // DeepFlow parameters
    int m_df_nstep_size ;
    int m_df_nmin_mv_size ;
    // For video capture
    cv::VideoCapture cap;
    // Video frame in type of cv::Mat
    cv::Mat mat_cur_frame ;
    cv::Mat mat_pre_frame ;
    // Video frame in type of QImage
    QImage qim_original ;
    // Frame motion vectors
    //std::vector<MotionVectorExtractor::stMotionVector> stmotionvec_frame ;

    QTimer* tmrTimer ;

    void Initialize( void ) ;
    void Initialize_LK_Values( void ) ;
    void Initialize_Farneback_Values( void ) ;
    void Initialize_SimpleFlow_Values( void ) ;
    void Initialize_DeepFlow_Values( void ) ;
    void SetMVExtractorParamValues( void ) ;
    void DrawLine( cv::Mat& frame, std::vector<MotionVectorExtractor::stMotionVector>& stmv_frame ) ;

public slots:
    void ProcessFrameAndUpdateGUI() ;

private slots:
    void on_StopResume_clicked();
    void on_OpenFile_clicked();
    // Lucas Kanade actions
    void on_LK_maxCorner_sliderMoved(int value);
    void on_LK_spinBox_maxCorner_valueChanged(int value );
    void on_LK_qualityLevel_valueChanged(int value);
    void on_LK_spinBox_qualityLevel_valueChanged(int value);
    void on_LK_BlockSize_valueChanged(int value);
    void on_LK_spinBox_BlockSize_valueChanged(int value);
    void on_LK_comboBox_currentIndexChanged(int index) ;
    void on_LK_Harris_k_valueChanged(int value);
    void on_LK_spinBox_Harris_k_valueChanged(int value);
    void on_LK_windowSize_valueChanged(int value);
    void on_LK_spinBox_windowSize_valueChanged(int value);
    void on_LK_maxPyramidLevel_valueChanged(int value);
    void on_LK_spinBox_maxPyramidLevel_valueChanged(int value);
    void on_LK_maxCountLevel_valueChanged(int value);
    void on_LK_spinBox_maxCountLevel_valueChanged(int value);
    void on_LK_epsilon_valueChanged(int value);
    void on_LK_spinBox_epsilon_valueChanged(int value);
    void on_LK_minEigThreshold_valueChanged(int value);
    void on_LK_spinBox_minEigThreshold_valueChanged(int value);
    void on_LK_setMinMVSize_valueChanged(int value);
    void on_LK_spinBox_setMinMVSize_valueChanged(int value);
    // Farneback actions
    void on_Farneback_stepSize_sliderMoved(int value);
    void on_Farneback_spinBox_stepSize_valueChanged(int value);
    void on_Farneback_maxPyramidScale_sliderMoved(int value);
    void on_Farneback_spinBox_maxPyramidScale_valueChanged(int value);
    void on_Farneback_pyramidLevel_sliderMoved(int value);
    void on_Farneback_spinBox_pyramidLevel_valueChanged(int value);
    void on_Farneback_winSize_sliderMoved(int value);
    void on_Farneback_spinBox_winSize_valueChanged(int value);
    void on_Farneback_iterations_sliderMoved(int value);
    void on_Farneback_spinBox_iterations_valueChanged(int value);
    void on_Farneback_poly_n_sliderMoved(int position);
    void on_Farneback_spinBox_poly_n_valueChanged(int value);
    void on_Farneback_poly_sigma_sliderMoved(int value);
    void on_Farneback_spinBox_poly_sigma_valueChanged(int value);
    void on_Farneback_setMinMVSize_sliderMoved(int value);
    void on_Farneback_spinBox_setMinMVSize_valueChanged(int value);
    // Simple flow
    void on_SimpleFlow_stepSize_sliderMoved(int value);
    void on_SimpleFlow_spinBox_stepSize_valueChanged(int value);
    void on_SimpleFlow_layers_sliderMoved(int value);
    void on_SimpleFlow_spinBox_layers_valueChanged(int value);
    void on_SimpleFlow_averaginBlockSize_sliderMoved(int value);
    void on_SimpleFlow_spinBox_averaginBlockSize_valueChanged(int value);
    void on_SimpleFlow_maxFlow_sliderMoved(int value);
    void on_SimpleFlow_spinBox_maxFlow_valueChanged(int value);
    void on_SimpleFlow_sigmaDist_sliderMoved(int value);
    void on_SimpleFlow_spinBox_sigmaDist_valueChanged(int value);
    void on_SimpleFlow_sigmaColor_sliderMoved(int value);
    void on_SimpleFlow_spinBox_sigmaColor_valueChanged(int value);
    void on_SimpleFlow_postWin_sliderMoved(int value);
    void on_SimpleFlow_spinBox_postWin_valueChanged(int value);
    void on_SimpleFlow_flowSigmaDistFix_sliderMoved(int value);
    void on_SimpleFlow_spinBox_flowSigmaDistFix_valueChanged(int value);
    void on_SimpleFlow_flowSigmaColorFix_sliderMoved(int value);
    void on_SimpleFlow_spinBox_flowSigmaColorFix_valueChanged(int value);
    void on_SimpleFlow_occThr_sliderMoved(int value);
    void on_SimpleFlow_spinBox_occThr_valueChanged(int value);
    void on_SimpleFlow_upscaleAvgRadius_sliderMoved(int value);
    void on_SimpleFlow_spinBox_upscaleAvgRadius_valueChanged(int value);
    void on_SimpleFlow_upscaleSigmaDist_sliderMoved(int value);
    void on_SimpleFlow_spinBox_upscaleSigmaDist_valueChanged(int value);
    void on_SimpleFlow_upscaleSigmaColor_sliderMoved(int value);
    void on_SimpleFlow_spinBox_upscaleSigmaColor_valueChanged(int value);
    void on_SimpleFlow_speedupThreshold_sliderMoved(int value);
    void on_SimpleFlow_spinBox_speedupThreshold_valueChanged(int value);
    void on_SimpleFlow_setMinMVSize_sliderMoved(int value);
    void on_SimpleFlow_spinBox_setMinMVSize_2_valueChanged(int value);
    // Deep flow
    void on_DeepFlow_stepSize_sliderMoved(int value);
    void on_DeepFlow_spinBox_stepSize_valueChanged(int value);
    void on_DeepFlow_setMinMVSize_sliderMoved(int value);
    void on_DeepFlow_spinBox_setMinMVSize_valueChanged(int value);
};
#endif // MAINWINDOW_H
