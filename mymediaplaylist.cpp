#include "mymediaplaylist.h"

MyMediaPlaylist::MyMediaPlaylist(QObject *parent)
    : QObject(parent), currentIndex_(-1) {
    // 构造函数，初始化当前索引为 -1（无有效索引）
}

void MyMediaPlaylist::addMedia(const QUrl &media) {
    // 向列表中添加新的媒体资源
    mediaList_.append(media);
    // 如果之前没有有效索引，新增媒体后将当前索引设为0，并触发信号
    if (currentIndex_ == -1) {
        currentIndex_ = 0;
        emit currentIndexChanged(currentIndex_);
    }
}

void MyMediaPlaylist::clear() {
    // 清空媒体列表，并重置索引
    mediaList_.clear();
    currentIndex_ = -1;
    emit currentIndexChanged(currentIndex_);
}

int MyMediaPlaylist::currentIndex() const {
    // 返回当前播放索引
    return currentIndex_;
}

void MyMediaPlaylist::setCurrentIndex(int index) {
    // 设置当前播放索引，条件是索引有效且与当前不同
    if (index >= 0 && index < mediaList_.size() && index != currentIndex_) {
        currentIndex_ = index;
        emit currentIndexChanged(currentIndex_);
    }
}

QUrl MyMediaPlaylist::currentMedia() const {
    // 返回当前播放的媒体 URL，如果索引无效返回空 QUrl
    if (currentIndex_ >= 0 && currentIndex_ < mediaList_.size())
        return mediaList_.at(currentIndex_);
    return QUrl();
}

QUrl MyMediaPlaylist::nextMedia() {
    // 切换到下一条媒体，循环播放（到达末尾后返回开头）
    if (mediaList_.isEmpty())
        return QUrl();

    currentIndex_ = (currentIndex_ + 1) % mediaList_.size();
    emit currentIndexChanged(currentIndex_);
    return mediaList_.at(currentIndex_);
}

QUrl MyMediaPlaylist::previousMedia() {
    // 切换到上一条媒体，循环播放（到达开头后返回末尾）
    if (mediaList_.isEmpty())
        return QUrl();

    currentIndex_ = (currentIndex_ - 1 + mediaList_.size()) % mediaList_.size();
    emit currentIndexChanged(currentIndex_);
    return mediaList_.at(currentIndex_);
}

int MyMediaPlaylist::mediaCount() const {
    // 返回播放列表总数
    return mediaList_.size();
}
