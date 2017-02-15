<GameFile>
  <PropertyGroup Name="LotteryActionLayer" Type="Layer" ID="aeae0670-b0b5-4c99-9a04-31c0e9330726" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="280" Speed="1.0000">
        <Timeline ActionTag="-1401497465" Property="Position">
          <PointFrame FrameIndex="0" X="270.0000" Y="500.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="140" X="270.0000" Y="500.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="280" X="270.0000" Y="500.0000">
            <EasingData Type="0" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="-1401497465" Property="Scale">
          <ScaleFrame FrameIndex="0" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="140" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="280" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-1401497465" Property="RotationSkew">
          <ScaleFrame FrameIndex="0" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="140" X="180.0000" Y="180.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="280" X="360.0000" Y="360.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-1401497465" Property="AnchorPoint">
          <ScaleFrame FrameIndex="0" X="0.4832" Y="0.4733">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="140" X="0.4832" Y="0.4733">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="280" X="0.4832" Y="0.4733">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
      </Animation>
      <ObjectData Name="Layer" Tag="56" ctype="GameLayerObjectData">
        <Size X="540.0000" Y="960.0000" />
        <Children>
          <AbstractNodeData Name="action1" ActionTag="-1401497465" Tag="57" IconVisible="False" LeftMargin="36.6144" RightMargin="20.3856" TopMargin="205.0772" BottomMargin="270.9228" ctype="SpriteObjectData">
            <Size X="483.0000" Y="484.0000" />
            <AnchorPoint ScaleX="0.4832" ScaleY="0.4733" />
            <Position X="270.0000" Y="500.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5208" />
            <PreSize X="0.8944" Y="0.5042" />
            <FileData Type="MarkedSubImage" Path="UI/action1.png" Plist="ui.plist" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="LotteryActionBtn" ActionTag="-624979639" Tag="58" IconVisible="False" LeftMargin="165.9999" RightMargin="166.0001" TopMargin="793.5041" BottomMargin="73.4959" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="178" Scale9Height="71" OutlineSize="0" ShadowOffsetX="0.0000" ShadowOffsetY="0.0000" ctype="ButtonObjectData">
            <Size X="208.0000" Y="93.0000" />
            <Children>
              <AbstractNodeData Name="oktext" ActionTag="911592896" Tag="62" IconVisible="False" LeftMargin="52.4985" RightMargin="50.5015" TopMargin="23.0000" BottomMargin="24.0000" ctype="SpriteObjectData">
                <Size X="105.0000" Y="46.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="104.9985" Y="47.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5048" Y="0.5054" />
                <PreSize X="0.5048" Y="0.4946" />
                <FileData Type="MarkedSubImage" Path="UI/oktext.png" Plist="ui.plist" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="269.9999" Y="119.9959" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.1250" />
            <PreSize X="0.3852" Y="0.0969" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="MarkedSubImage" Path="UI/rewardbtn0.png" Plist="ui.plist" />
            <PressedFileData Type="MarkedSubImage" Path="UI/rewardbtn0.png" Plist="ui.plist" />
            <NormalFileData Type="MarkedSubImage" Path="UI/rewardbtn0.png" Plist="ui.plist" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>