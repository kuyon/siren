﻿/* THIS FILE IS PART OF *SIREN* SOURCE CODES.
 * WHEN YOU WANT THE LICENSE OF THIS FILE, YOU CAN GET IT
 * IN README OR LICENSE DOCMENTATION OF THIS PROJECT.
 * ---------------------------------------------------------
 * AUTHOR: dyama <dyama@member.fsf.org>
 */

using System;
using System.Drawing;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Windows.Forms;
using sirenenv;

namespace siren
{
    public enum MouseState : int { DOWN = -1, UP = 1 }
	public enum CurAct3d { NOTHING, ZOOM, WINZOOM, PAN, GLOPAN, ROTATE }

    public partial class ViewForm : System.Windows.Forms.Form
    {
        protected CurAct3d myCurMode;
		protected float myCurZoom;// ~ Quantity_Factor
		protected bool myDegenerateModeIsOn;
		protected int myXmin;
		protected int myYmin;
		protected int myXmax;
		protected int myYmax;
		protected int theButtonDownX;
		protected int theButtonDownY;
        // 矩形削除用
		protected int theRectDownX;
		protected int theRectDownY;
		protected bool IsRectVisible;

        protected void initMouseEvent()
        {
			myCurMode = CurAct3d.NOTHING;
			myDegenerateModeIsOn = true;
			IsRectVisible = false;

            return;
        }
        
        void onMouseEnter(object sender, EventArgs e)
        {
            this.Focus();
        }

        private void onMouseWheel(object sender, MouseEventArgs e)
        {
            double factor = e.Delta * SystemInformation.MouseWheelScrollLines / 200;
            double scale = myViewer.Scale();

            if (scale > 0.01)
            {
              factor /= 100;
            }
            else
            {
              factor /= 1000;
            }

            if (scale - factor < 0)
                scale = 0.001;
            else
                scale = scale - factor;

            myViewer.SetScale(scale);
#if DEBUG
            Console.WriteLine(scale + ", " + factor);
#endif
        }  

        void onMouseDoubleClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Middle)
                myViewer.ZoomAllView();
        }

		protected void DragEvent(int x, int y, MouseState theState)
		{
			if (theState == MouseState.DOWN) {
				theButtonDownX = x;
				theButtonDownY = y;
			} 
			else if (theState == MouseState.UP) {
				myViewer.Select(Math.Min(theButtonDownX, x), Math.Min(theButtonDownY, y),
					Math.Max(theButtonDownX, x), Math.Max(theButtonDownY, y));
			}
		}

		private void DrawRectangle(bool draw)
		{
			Graphics gr = Graphics.FromHwnd(this.Handle);
			System.Drawing.Pen p = null;

			if (this.IsRectVisible ||(!draw))//erase the rect
			{
				int r = myViewer.GetBGColR();
				int g = myViewer.GetBGColG();
				int b = myViewer.GetBGColB();
				p = new Pen(System.Drawing.Color.FromArgb(r,g, b));
				this.IsRectVisible = false;
				this.myViewer.UpdateView();
			} 
			else if (draw) 
			{
				p = new Pen(System.Drawing.Color.White);
				this.IsRectVisible = true;
			}

			if (p == null)
				return;

            this.myViewer.UpdateView();

			int x = Math.Min(this.myXmin, this.myXmax);
			int y = Math.Min(this.myYmin, this.myYmax);
			gr.DrawRectangle(p, x, y, Math.Abs(myXmax - myXmin), Math.Abs(myYmax - myYmin));
			this.theRectDownX = Math.Max(this.myXmin, this.myXmax);
			this.theRectDownY = Math.Max(this.myYmin, this.myYmax);
		}

		private void onMouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			switch (e.Button) 
			{
			case MouseButtons.Left:
				myXmin=e.X;	myYmin=e.Y;
				myXmax=e.X;	myYmax=e.Y;
                if (myCurSpKey == CurSpKey.CTRL)
                {
                    // start the dinamic zooming....
                    this.Cursor = System.Windows.Forms.Cursors.SizeWE;
                    myCurMode = CurAct3d.ZOOM;
                }
                else
                {
                    switch (myCurMode)
                    {
                        case CurAct3d.NOTHING:
                            DragEvent(myXmax, myYmax, MouseState.DOWN);
                            break;
                        case CurAct3d.ROTATE:
                            if (!myDegenerateModeIsOn)
                                myViewer.SetDegenerateModeOn();
                            //start the rotation
                            myViewer.StartRotation(e.X, e.Y);
                            break;
                        case siren.CurAct3d.WINZOOM:
                            this.Cursor = System.Windows.Forms.Cursors.Hand;
                            break;
                        default:
                            break;
                    }
                }
				break;

			case MouseButtons.Right:
				if (!myDegenerateModeIsOn)
					myViewer.SetDegenerateModeOn();
                this.Cursor = System.Windows.Forms.Cursors.SizeAll;
				myViewer.StartRotation(e.X, e.Y);
				break;

            case MouseButtons.Middle:
                this.Cursor = System.Windows.Forms.Cursors.NoMove2D;
                break;
			default:
				break;
			}

		}

		private void onMouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			if (e.Button == MouseButtons.Left) {
				if (myCurSpKey==CurSpKey.CTRL) {
					myViewer.Zoom(myXmax, myYmax, e.X, e.Y);
					myXmax=e.X; myYmax=e.Y;
				}
				else {
					switch (myCurMode) {
					case CurAct3d.NOTHING:
						DrawRectangle(false);
						myXmax=e.X; myYmax=e.Y;
						DrawRectangle(true);
						break;
					case CurAct3d.ZOOM:
						myViewer.Zoom(myXmax, myYmax, e.X, e.Y);
						myXmax=e.X; myYmax=e.Y;
						break;
					case CurAct3d.WINZOOM:
						DrawRectangle(false);
						myXmax=e.X; myYmax=e.Y;
						DrawRectangle(true);//add brush here
						break;
					case CurAct3d.PAN:
						myViewer.Pan(e.X-myXmax, myYmax - e.Y);
						myXmax=e.X; myYmax=e.Y;
						break;
					case CurAct3d.GLOPAN:
						break;
					case CurAct3d.ROTATE:
						myViewer.Rotation(e.X, e.Y);
						myViewer.RedrawView();
						break;
					default:
						break;
					}
				}
			}
			else if (e.Button==MouseButtons.Middle) {
				myViewer.Pan(e.X-myXmax, myYmax - e.Y);
				myXmax=e.X; myYmax=e.Y;
			}
			else if	(e.Button==MouseButtons.Right) {
					myViewer.Rotation(e.X, e.Y);
			}
			else {
				myXmax=e.X; myYmax=e.Y;
                myViewer.MoveTo(e.X, e.Y);
			}
            return;
		}

		private void onMouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			switch(e.Button)
			{
			case MouseButtons.Left:
                if (myCurSpKey == CurSpKey.CTRL)
                {
                    this.Cursor = System.Windows.Forms.Cursors.Default;
                    return;
                }
				switch(myCurMode)
				{
				case CurAct3d.NOTHING:
					if(e.X == myXmin && e.Y == myYmin) {
						myXmax = e.X;
                        myYmax = e.Y;
                        if (myCurSpKey == CurSpKey.SHIFT)
                            myViewer.ShiftSelect();
                        else
                            myViewer.Select();
					} 
					else {
						myXmax=e.X; myYmax=e.Y;
						DrawRectangle(false);
						DragEvent(myXmax, myYmax, MouseState.UP);
					}
					break;

				case CurAct3d.ZOOM:
					myCurMode = CurAct3d.NOTHING;
					break;

				case CurAct3d.WINZOOM:
					myXmax = e.X;
                    myYmax = e.Y;
					DrawRectangle(false);
					int ValZWMin = 1;
					if (Math.Abs(myXmax-myXmin)>ValZWMin && Math.Abs(myXmax-myYmax)>ValZWMin)
						myViewer.WindowFitAll(myXmin, myYmin, myXmax, myYmax);
					this.Cursor = System.Windows.Forms.Cursors.Default;
					siren.MainForm f = (siren.MainForm)this.ParentForm;
					myCurMode = CurAct3d.NOTHING;
					break;

				case CurAct3d.PAN:
					myCurMode = CurAct3d.NOTHING;
					break;

				case CurAct3d.GLOPAN:
					myViewer.Place(e.X, e.Y, myCurZoom);
					myCurMode = CurAct3d.NOTHING;
					break;

				case CurAct3d.ROTATE:
					myCurMode = CurAct3d.NOTHING;
					if (!myDegenerateModeIsOn) {
						myViewer.SetDegenerateModeOff();
						myDegenerateModeIsOn=false;
					} 
					else {
						myViewer.SetDegenerateModeOn();
						myDegenerateModeIsOn=true;
					}
					break;

				default:
					break;

				}
				break;

			case MouseButtons.Right:
				if (!myDegenerateModeIsOn) {
					myViewer.SetDegenerateModeOff();
					myDegenerateModeIsOn=false;
				} 
				else {
					myViewer.SetDegenerateModeOn();
					myDegenerateModeIsOn=true;
				}
				break;

			default:
				break;
			}
			
			this.Cursor = System.Windows.Forms.Cursors.Default;
			siren.MainForm parent = (siren.MainForm)this.ParentForm;

            return;
		}

    }
}